#ifndef FFTALGORITHM_H
#define FFTALGORITHM_H
#include <QtMath>

template<unsigned M, unsigned N, unsigned B, unsigned A>
struct SinCosSeries {
   static double value() {
      return 1-(A*M_PI/B)*(A*M_PI/B)/M/(M+1)
               *SinCosSeries<M+2,N,B,A>::value();
   }
};

template<unsigned N, unsigned B, unsigned A>
struct SinCosSeries<N,N,B,A> {
   static double value() { return 1.; }
};


template<unsigned B, unsigned A, typename T=double>
struct Sin;

template<unsigned B, unsigned A>
struct Sin<B,A,double> {
   static double value() {
      return (A*M_PI/B)*SinCosSeries<2,34,B,A>::value();
   }
};

template<unsigned B, unsigned A, typename T=double>
struct Cos;

template<unsigned B, unsigned A>
struct Cos<B,A,double> {
   static double value() {
      return SinCosSeries<1,33,B,A>::value();
   }
};

template<unsigned N, typename T=double>
class FftAlgorithm {
   FftAlgorithm<N/2,T> next;
public:
   void apply(T* data) {
      next.apply(data);
      next.apply(data+N);

      T wtemp,tempr,tempi,wr,wi,wpr,wpi;
      wtemp = -Sin<N,1,T>::value();
      wpr = -2.0*wtemp*wtemp;
      wpi = -Sin<N,2,T>::value();
      wr = 1.0;
      wi = 0.0;
      for (unsigned i=0; i<N; i+=2) {
        tempr = data[i+N]*wr - data[i+N+1]*wi;
        tempi = data[i+N]*wi + data[i+N+1]*wr;
        data[i+N] = data[i]-tempr;
        data[i+N+1] = data[i+1]-tempi;
        data[i] += tempr;
        data[i+1] += tempi;

        wtemp = wr;
        wr += wr*wpr - wi*wpi;
        wi += wi*wpr + wtemp*wpi;
      }
   }
   void applySlow(T* data)
   {
       unsigned long n, mmax, m, j, istep, i;
       T wtemp, wr, wpr, wpi, wi, theta;
       T tempr, tempi;

       // reverse-binary reindexing
       n = N<<1;
       j=1;
       for (i=1; i<n; i+=2) {
           if (j>i) {
               double temp = data[j-1];
               data[j-1] = data[i-1];
               data[i-1] = temp;
               temp = data[j];
               data[j] = data[i];
               data[i] = temp;
           }
           m = N;
           while (m>=2 && j>m) {
               j -= m;
               m >>= 1;
           }
           j += m;
       };

       // here begins the Danielson-Lanczos section
       mmax=2;
       while (n>mmax) {
           istep = mmax<<1;
           theta = -(2*M_PI/mmax);
           wtemp = sin(0.5*theta);
           wpr = -2.0*wtemp*wtemp;
           wpi = sin(theta);
           wr = 1.0;
           wi = 0.0;
           for (m=1; m < mmax; m += 2) {
               for (i=m; i <= n; i += istep) {
                   j=i+mmax;
                   tempr = wr*data[j-1] - wi*data[j];
                   tempi = wr * data[j] + wi*data[j-1];

                   data[j-1] = data[i-1] - tempr;
                   data[j] = data[i] - tempi;
                   data[i-1] += tempr;
                   data[i] += tempi;
               }
               wtemp=wr;
               wr += wr*wpr - wi*wpi;
               wi += wi*wpr + wtemp*wpi;
           }
           mmax=istep;
       }
   }
};

template<>
class FftAlgorithm<1> {
public:
    void apply(double* data) {
        return;
    }

    void applySlow(double* data)
    {
        return;
    }
};

#endif // FFTALGORITHM_H
