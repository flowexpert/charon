#!/usr/bin/gnuplot

# plot probability function and derivatives
set title "GSM probability function"

fname = "fit3_GSM_values.dat"

set term pdfcairo color solid
set output "fit3_GSM_prob.pdf"

plot \
	fname using 1:2 w l t "p",\
	fname using 1:3 w l t "p'",\
	fname using 1:4 w l t "p''"


set term pdfcairo color solid
set output "fit3_GSM_energy.pdf"

plot \
	fname using 1:5 w l t "E",\
	fname using 1:6 w l t "E'",\
	fname using 1:7 w l t "E''",\
	fname using 1:8 w l t "E'lin"
