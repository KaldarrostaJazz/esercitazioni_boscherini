f(x) = h * exp( - (x - u)**2 / (2 * s**2))
h = 1.6
u = 0.1
s = 1.
fit [-5:5] f(x) 'gaussiana1.txt' u 1:2:3 yerror via h, u, s

set multiplot title "Grafico"
set grid
set key
unset title

set origin 0, 0.34
set size 0.96, 0.64
unset xlabel
set format x ""
set ylabel "Asse Y"
plot 'gaussiana1.txt' u 1:2:3 w yerrorbars title "dati osservati", f(x) w l title "fit"

set origin 0.011, 0
set size 0.949, 0.38
unset ylabel
unset format x
set xlabel "Asse X"
set ytics ("-0.1" -0.1, "-0.05" -0.05, "0" 0, "0.05" 0.05, "0.1" 0.1)
set yrange [-0.15:0.15]
plot 'gaussiana1.txt' u 1:($2-f($1)) w l linetype 2 title "residui"
