#ifndef _CWT_HPP_
#define _CWT_HPP_

#include <vector>
#include <string>

#include <MotherWaveletGenerator.hpp>
#include <Matrix.hpp>

class Cwt {
public:
    Cwt(const std::string &mother);

    Matrix<double> GetTransform(void){return(transform);};
    std::vector<double> GetProjection(const std::string &par);
    
    void SetFinalTime(double a) {fTime = a; width = a - iTime;};
    void SetNumPoints(int a) {numPoints = a;};
    void SetNumScale(int a) {numScale = a;};
    void SetNumTau(int a) {numTau = a;};
    void SetInitialScale(double a) {iScale = a; scale = a;};
    void SetInitialTau(double a) {iTau = a; tau = a;};
    void SetInitialTime(double a) {iTime = a;};

    void Transform(std::vector<double>* signal);
private:
    double iTime, fTime, width;
    double tau, dTau, iTau;
    double scale, dScale, iScale;
    int numPoints, numSignal, numScale, numTau, timeStep;

    Matrix<double> transform;
    MotherWaveletGenerator mom;
};
#endif //_CWT_HPP_
