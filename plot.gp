set terminal wxt

set view 49, 321
set auto y
set auto x
#set xrange[1e-5:1e-4]
#set yrange[100:200]
set zlabel 'Norm Psi'
set xlabel 'Scale'
set ylabel 'Tau'
set hidden3d

splot 'trans.dat' with lines