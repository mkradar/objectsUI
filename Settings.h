#ifndef SETTINGS_H
#define SETTINGS_H
#include <QList>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

template<unsigned TicksPerPeriod, unsigned PeriodsPerCollection>
class Settings
{
public:
    void LoadFromFile(const QString& fileName)
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
            return;
        }

        QTextStream in(&file);

        unsigned int lineCounter = 0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            switch(lineCounter)
            {
            case 0:
                this->rangeCount = line.toInt();
                break;
            case 1:
                this->modulationFrequency = line.toDouble();
                break;
            case 2:
                this->frequencyDeviation = line.toDouble();
                break;
            case 3:
                this->minRange = line.toInt();
                break;
            case 4:
                this->maxRange = line.toInt();
                break;
            case 5:
                this->rangeIncrement = line.toInt();
                break;
            case 6:
                this->harmonicsCount = line.toInt();
                break;
            default:
                this->baseCoefficients.append(line.toDouble());
                break;
            }

            lineCounter++;
        }

        if(this->baseCoefficients.size() != 2 * this->rangeCount * this->harmonicsCount)
        {
            QMessageBox::information(0, "error", "Wrong number of base coefficients. Expected: "
                                     + QString(2 * this->rangeCount * this->harmonicsCount)
                                     + " Actual: "
                                     + QString(this->baseCoefficients.size()));
        }
    }

    int rangeCount;
    double modulationFrequency;
    double frequencyDeviation;
    int harmonicsCount;
    QList<double> baseCoefficients;
    int minRange;
    int rangeIncrement;

private:
    int maxRange;
};

#endif // SETTINGS_H
