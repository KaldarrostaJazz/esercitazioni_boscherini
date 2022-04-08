h1 = 5
h2 = 4
u1 = 24 
u2 = 26.5
s1 = 1
s2 = 1.2
f(x) = h1 * exp(- (x - u1)**2 / (2 * s1**2)) + h2 * exp(- (x -u2)**2 / (2 * s2**2))
fit [20:30] f(x) 'duegaussiane.txt' u 1:2:3 yerrors via h1, u1, s1, h2, u2, s2

set multiplot title "Grafico"
set grid
set key
set xrange [19:31]
unset title

set origin 0, 0.34
set size 0.96, 0.64
unset xlabel
set format x ""
set ylabel "Asse Y"
plot 'duegaussiane.txt' u 1:2:3 w yerrorbars title "Dati", [20:30] f(x) w l title "Fit"

set origin 0, 0
set size 0.96, 0.38
unset ylabel
unset format x
set xlabel "Asse X"
#set ytics ("-0.2" -0.2, "-0.1" -0.1, "0" 0, "0.1" 0.1, "0.2", 0.2)
set yrange [-0.25:0.25]
plot 'duegaussiane.txt' u 1:($2-f($1)) w l linetype 2 title "residui"
