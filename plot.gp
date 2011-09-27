set terminal wxt

set view 49, 321
set auto y
set xrange[0.00001:0.0001]
set yrange[200:300]
set zlabel 'Norm Psi'
set xlabel 'Scale'
set ylabel 'Tau'
set hidden3d

splot 'trans.dat' with lines