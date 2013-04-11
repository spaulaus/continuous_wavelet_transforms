#include <iostream>
#include <vector>

#include <cmath>

#include "ContinuousWaveletTransform.hpp"

using namespace std;

int main(void)
{
   double transform [noScale][noTau];
   vector<double> *signal = gen.GetSignal();
   
   //Do the loop for the transformation.
   for (int i = 0; i < noScale; i++, scale += dScale) { // Scaling
      tau = iTime;
      for (int j = 0; j < noTau; j++, tau+=dTau) { // Translation
    	 transform[i][j] = DoTransform(scale, tau, signal);
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
      for (int i = 0; i < noScale; i++, scale += dScale) { // Scaling
    	 tau = iTime;
    	 for (int j = 0; j < noTau; j++, tau+=dTau) { // Translation
    	    cout << scale << " " << tau << " " 
    		 << fabs(transform[i][j]/maximum) << endl;
    	 }
    	 cout << endl;
      }
   }// if(outputSignal)
   
    //Project out the Scale parameter
   double tauSum[noTau];
   for (int j = 0; j < noTau; j++) { // Translation
      scale = iScale;
      for (int i = 0; i < noScale; i++) { // Scaling
    	 if(i < 10)
    	    tauSum[j] += fabs(transform[i][j]/maximum);
      }
   }
   
   tau = iTau;
   for (int j = 0; j < noTau; j++, tau+=dTau) { // Translation
      cout << tau << " " << tauSum[j] << endl;
   }
}

double DoTransform(double &scale, double &tau, vector<double>* signal) {
   double integral = 0., t = iTime;
   for (vector<double>::iterator it = signal->begin();
	it != signal->end(); it++, t += h)
      integral += (*it)*mom.GetValue(t,scale,tau)*h;
   return (integral / sqrt(scale));
}
