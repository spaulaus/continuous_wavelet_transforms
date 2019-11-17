#include <iostream>

#include <cmath>

#include "Cwt.hpp"

using namespace std;

Cwt::Cwt(const string &mother) {
    mom.SetWaveletType(mother);
    iTime = 0.0;
    iTau  = 0.0;
}

void Cwt::Transform(vector<double>* signal) {
    // Need *very* small s steps for high-frequency, but only if s is small
    // Thus increment s by multiplying by number close enough to 1 
    //double dTau = width/noTau, dScale = pow(width/iScale, 1./noScale);
    
    transform.SetSize(numScale,numTau); //resize the transform matrix
    dTau = width/numTau; //calculate the step size for tau
    dScale = (1e-4-iScale)/numScale; // calculate the step size for the scale
    width = fTime - iTime; //calculate the width of the time domain
    timeStep = width/numPoints; //calculate the step size for the time domain
    
    double integral = 0., t = iTime; // set the initial values for the loops
    //int countScale, countTau;
    //Do the loop for the transformation.
    for (int i = 0; i < numScale; i++, scale += dScale) { // Scaling
        tau = iTime;
        for (int j = 0; j < numTau; j++, tau+=dTau) { // Translation
            for (vector<double>::iterator it = signal->begin();
                 it != signal->end(); it++, t += timeStep)
                integral += (*it)*mom.GetValue(t,scale,tau)*timeStep;
            transform(i,j) = integral/sqrt(scale);
            cout << scale << " " << tau << " " << transform(i,j) << endl;
        }
    }

    //Find the maximum value of the transformation
    //for normalization
    double maximum = 0.001;
    for (int i = 0; i < numScale; i++) { // Scaling
        for (int j = 0; j < numTau; j++) { // Translation
            if(transform(i,j) > maximum || transform(i,j) < -1*maximum)
                maximum = fabs(transform(i,j));
        }
    }
    
    tau = iTime; scale = iScale;
    for (int i = 0; i < numScale; i++, scale += dScale) { // Scaling
        tau = iTime;
        for (int j = 0; j < numTau; j++, tau+=dTau) // Translation
            transform(i,j) /= fabs(maximum);
    }
}

vector<double> Cwt::GetProjection(const string &par) {
    //Project out the Scale parameter
    double tauSum[numTau];
    for (int j = 0; j < numTau; j++) { // Translation
        scale = iScale;
        for (int i = 0; i < numScale; i++) { // Scaling
            if(i < 10)
                tauSum[j] += transform(i,j);
        }
    }
    
    tau = iTau;
    for (int j = 0; j < numTau; j++, tau+=dTau) { // Translation
        cout << tau << " " << tauSum[j] << endl;
    }
    vector<double> temp;
    return(temp);
}
