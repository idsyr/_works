set datafile separator comma
set title "outfile graph"

set xlabel "t"
set ylabel "filtered_samples"
set grid

plot 'magni_out.dat' using 1 with lines title 'filtered_samples', \
     ''              using 2 with lines title 'spark'

