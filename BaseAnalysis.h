#ifndef BASEANALYSIS_H
#define BASEANALYSIS_H
#include "Settings.h"
#include "Signals.h"
#include "FftAlgorithm.h"
#include "RadarException.h"
#include <QDebug>

template<unsigned TicksPerPeriod, unsigned PeriodsPerCollection>
class BaseAnalysis
{
private:
    double* _complexSignals = NULL;
    double* _correlationPerPeriodBase = NULL;
    double* _correlationPerPeriodSquared = NULL;
    double* _correlationOverall = NULL;
    Settings<TicksPerPeriod, PeriodsPerCollection>* _settings;
    Signals* _signals;

public:
    ~BaseAnalysis()
    {
        if(this->_complexSignals != NULL)
        {
            delete _complexSignals;
        }

        if(this->_correlationPerPeriodBase != NULL)
        {
            delete _correlationPerPeriodBase;
        }

        if(this->_correlationPerPeriodSquared != NULL)
        {
            delete _correlationPerPeriodSquared;
        }

        if(this->_correlationOverall != NULL)
        {
            delete _correlationOverall;
        }
    }


    BaseAnalysis(Settings<TicksPerPeriod, PeriodsPerCollection> &settings, Signals& signal)
    {
        this->_settings = &settings;
        this->_signals = &signal;
    }

    void calculate()
    {
        this->applyFFT();
        this->correlationsPerPeriod();
        double correlationBase[this->_settings->rangeCount];
        double correlationSquared[this->_settings->rangeCount];
        this->correlationOverall(this->_correlationPerPeriodBase, &correlationBase[0]);
        this->correlationOverall(this->_correlationPerPeriodSquared, &correlationSquared[0]);
        this->correlationFinal(&correlationBase[0], &correlationSquared[0]);
        int rangeIndex = this->detectBiggest(this->_correlationOverall, this->_settings->rangeCount);
        double maxValue = this->_correlationOverall[rangeIndex];
        for(int i = 0; i < this->_settings->rangeCount; i++)
        {
            this->_correlationOverall[i] = 1 - this->_correlationOverall[i]/maxValue;
            // qDebug() << i << ": " << 1 - this->_correlationOverall[i];
        }

        qDebug() << "detected range: " << this->_settings->minRange + this->_settings->rangeIncrement * rangeIndex;
        qDebug() << "with value: " << maxValue;
    }

    double getAt(int index)
    {
        return this->_correlationOverall[index];
    }

private:
    int detectBiggest(double* array, int size)
    {
        int rangeIndex = 0;
        double currentMax = 0.0;
        for(int i = 0; i < size; i++)
        {
            if(*(array + i) > currentMax)
            {
                currentMax = *(array + i);
                rangeIndex = i;
            }
        }

        return rangeIndex;
    }

    void applyFFT()
    {
        int signalsCount = TicksPerPeriod * PeriodsPerCollection;

        this->_complexSignals = new double[signalsCount * 2];
        for(int i = 0; i < signalsCount; i++)
        {
            this->_complexSignals[2*i] = this->_signals->getAt(i);
            this->_complexSignals[2*i + 1] = 0;
        }

        FftAlgorithm<TicksPerPeriod> algo;
        for(int i = 0; i < PeriodsPerCollection; i++)
        {
            algo.applySlow(this->_complexSignals + TicksPerPeriod * i * 2);
        }
    }

    void correlationsPerPeriod()
    {
        this->_correlationPerPeriodBase = new double[this->_settings->rangeCount * PeriodsPerCollection];
        this->_correlationPerPeriodSquared = new double[this->_settings->rangeCount * PeriodsPerCollection];
        // iterator for collections
        for(int i = 0; i < PeriodsPerCollection; i++)
        {
            int fftOffset = 2*i*TicksPerPeriod;
            // iterator for range cell in a single collection
            for(int j = 0; j < this->_settings->rangeCount; j++)
            {
                int baseOffset = 2*j*this->_settings->harmonicsCount;
                int index = i*this->_settings->rangeCount + j;
                this->_correlationPerPeriodBase[index] = 0;
                this->_correlationPerPeriodSquared[index] = 0;
                // iterate through base coefficients
                for(int k = 0; k < this->_settings->harmonicsCount; k++)
                {
                    double realFft = this->_complexSignals[2*k + fftOffset];
                    double realBase = this->_settings->baseCoefficients[2*k + baseOffset];
                    double imageFft = this->_complexSignals[2*k + 1 + fftOffset];
                    double imageBase = this->_settings->baseCoefficients[2*k + 1 + baseOffset];
                    double multipliedValue = realFft*realBase - imageFft*imageBase;

                    k%2 == 0 ? this->_correlationPerPeriodBase[index] += multipliedValue : this->_correlationPerPeriodSquared[index] += multipliedValue;
                }
            }
        }
    }

    void correlationOverall(double* correlationPerPeriod, double* correlation)
    {
        // iterator for range cells
        for(int i = 0; i < this->_settings->rangeCount; i++)
        {
            correlation[i] = 0;
            double correlationPerCell[2 * PeriodsPerCollection];
            // select all perPeriod correlations for a single range cell
            for(int j = 0; j < PeriodsPerCollection; j++)
            {
                correlationPerCell[2*j] = correlationPerPeriod[j * this->_settings->rangeCount + i];
                // zero imaginary part
                correlationPerCell[2*j + 1] = 0;
            }

            FftAlgorithm<PeriodsPerCollection> algo;
            algo.applySlow(correlationPerCell);

            double squaredFft[PeriodsPerCollection];
            for(int j = 0; j < PeriodsPerCollection; j++)
            {
                squaredFft[j] = sqrt(pow(correlationPerCell[2*j],2) + pow(correlationPerCell[2*j + 1],2));
                // find max
                if(squaredFft[j] > correlation[i])
                {
                    correlation[i] = squaredFft[j];
                }
            }
        }
    }

    void correlationFinal(double* correlationBase, double* correlationSquared)
    {
        this->_correlationOverall = new double[this->_settings->rangeCount];
        for(int i = 0; i < this->_settings->rangeCount; i++)
        {
            this->_correlationOverall[i] = sqrt(pow(correlationBase[i],2) + pow(correlationSquared[i],2));
        }
    }
};

#endif // BASEANALYSIS_H
