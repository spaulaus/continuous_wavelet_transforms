#include <iostream>
#include <string>
#include <vector>

#include "Cwt.hpp"
#include <GnuplotPipes.hpp>
#include <Matrix.hpp>
#include <SignalGenerator.hpp>

using namespace std;

int main() {
    Gnuplot pipe("lines");
    SignalGenerator gen;
    gen.SetSignalType("sine");
    gen.SetAmplitude(1.0);
    gen.SetPeriod(1.0);
    gen.SetSignalResolution(0.5);
    gen.SetSignalLength(1000);
    gen.GenerateSignal();
    vector<double> *signal = gen.GetSignal();
    
    Cwt cwt("morlet");
    cwt.SetFinalTime(2000.0);
    cwt.SetNumPoints(2000.0);
    cwt.SetNumScale(10.0);
    cwt.SetNumTau(1e5);
    cwt.SetInitialScale(1e-5);
    cwt.SetInitialTau(0.);
    cwt.Transform(signal);
}
