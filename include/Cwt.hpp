#include "MotherWaveletGenerator.hpp"
#include "SignalGenerator.hpp"

class ContinousWaveletTransform {
public:

private:
   double DoTransform(double &scale, double &tau, vector<double>* signal);
   
   double iTime = 0.0, fTime = 2000.0, width = fTime - iTime;   //i,f times
   int noPoints = 2000; double h = width/noPoints;            //Steps
   int noPtsSignal = noPoints, noScale = 10, noTau = 1e5;    //# of pts
   double iTau = 0., iScale = 0.00001, tau = iTau, scale = iScale;
   // Need *very* small s steps for high-frequency, but only if s is small
   // Thus increment s by multiplying by number close enough to 1 
   //double dTau = width/noTau, dScale = pow(width/iScale, 1./noScale);
   double dTau = width/noTau, dScale = (1e-4-iScale)/noScale;
   bool outputSignal;
   
   MotherWaveletGenerator mom;
   mom.SetType("book");
}
