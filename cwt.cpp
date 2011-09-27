#include <fstream>
#include <iostream>
#include <vector>

#include <cmath>

using namespace std;

double MotherWavelet(const double &t, const double &scale, const double &tau);
double DoTransform(double &scale, double &tau);
void MakeSignal(void); 

int sigSource = 0, waveletType = 2;
bool outputSignal = false;

vector<double> signal; 
double iTime = 0.0, fTime = 2000.0, width = fTime - iTime;   //i,f times
int noPoints = 2000; double h = width/noPoints;            //Steps
int noPtsSignal = noPoints, noScale = 100, noTau = 1000;    //# of pts
double iTau = 0., iScale = 0.00001, tau = iTau, scale = iScale;
// Need *very* small s steps for high-frequency, but only if s is small
// Thus increment s by multiplying by number close enough to 1 
double dTau = width/noTau, dScale = pow(width/iScale, 1./noScale);

int main(void)
{
   double transform [noScale][noTau];
   MakeSignal();
   
   //Do the loop for the transformation.
   for (int i = 0; i < noScale; i++, scale *= dScale) { // Scaling
      tau = iTime;
      for (int j = 0; j < noTau; j++, tau+=dTau) { // Translation
   	 transform[i][j] = DoTransform(scale, tau);
      }
   }

   //Find the maximum value of the transformation
   //for normalization
   double maximum = 0.001;
   for (int i = 0; i < noScale; i++) { // Scaling
      for (int j = 0; j < noTau; j++) { // Translation
	 if(transform[i][j] > maximum || transform[i][j] < -1*maximum)
	    maximum = fabs(transform[i][j]);
      }
   }
   
   //Output either the signal or the source 
   if(outputSignal) {
      for(vector<double>::iterator it = signal.begin(); 
   	  it!= signal.end(); it++){
   	 cout << it - signal.begin() << " " << *it << endl;
      }
   }else {
      tau = iTime;
      scale = iScale;
      for (int i = 0; i < noScale; i++, scale *= dScale) { // Scaling
   	 tau = iTime;
   	 for (int j = 0; j < noTau; j++, tau+=dTau) { // Translation
   	    cout << scale << " " << tau << " " 
		 << fabs(transform[i][j]/maximum) << endl;
   	 }
   	 cout << endl;
      }
   }// if(outputSignal)
}

double DoTransform(double &scale, double &tau)  
{
   double integral = 0., t = iTime;
   for (vector<double>::iterator it = signal.begin();
	it != signal.end(); it++, t += h)
      integral += (*it)*MotherWavelet(t,scale,tau)*h;
   return (integral / sqrt(scale));
}

double MotherWavelet(const double &t, const double &scale, const double &tau)
{
   double psi = 0;
   double transVar = (t-tau)/scale;
   switch(waveletType) {
   case 0: // Book Example
      psi = sin(8*transVar) * exp(-transVar*transVar/2);
      break;
   case 1: {// Hermitian wavelet 
      psi = -1/(pow(2*M_PI, 2)*pow(scale,3))*transVar * 
	 exp(-transVar*transVar/2);
      break;
   }
   case 2: {//Mexican Hat
      psi = (2/(sqrt(3*scale)*pow(M_PI, 0.25)))*
      (1-(transVar*transVar))* 
      exp(-transVar*transVar/2);
      break;
   }
   case 3: //Morlet
      psi = cos(2*M_PI*transVar)*exp(-transVar*transVar/2);
      break;
   }
   return(psi);
}

void MakeSignal(void) 
{
   double t = 0.0; 
   double hs = width / noPtsSignal;
   
   switch(sigSource) {
   case 0: {
      ifstream readData("realData.dat");
      if(readData.is_open()) {
	 while(readData.good()) {
	    double junk0, junk1, junk2;
	    readData >> junk0 >> junk1 >> junk2;
	    signal.push_back(junk1);
	 }
	 readData.close();
      }
      break;
   }
   case 1:
      for(double i = 0; i < noPtsSignal; i++, t += hs )
	 signal.push_back(sin(2*M_PI*t));
      break;
   case 2:
      for (int i = 0; i < noPtsSignal; i++, t+=hs) {
	 double t1 = width/6., t2 = 4.*width/6.;       
	 if (t >= iTime && t <= t1) 
	    signal.push_back(sin(2*M_PI*t));
	 else if ( t >= t1 && t<=t2 ) 
	    signal.push_back(5.*sin(2*M_PI*t) + 10.*sin(4*M_PI*t));
	 else if ( t >= t2 && t <= fTime )
	    signal.push_back(2.5*sin(2*M_PI*t)+6.*sin(4*M_PI*t)+
			     10*sin(6*M_PI*t));
      }
      break;
   } //switch(source)
}
