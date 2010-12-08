#!/usr/bin/gnuplot

# plot probability function and derivatives
set title "GSM probability function"

fileName = "fit3_GSM_values.dat"

set term pdfcairo color solid
set output "fit3_GSM_prob.pdf"

plot \
	fileName using 1:2 with lines title "p",\
	fileName using 1:3 with lines title "p'",\
	fileName using 1:4 with lines title "p''"


set term pdfcairo color solid
set output "fit3_GSM_energy.pdf"

plot \
	fileName using 1:5 with lines title "E",\
	fileName using 1:6 with lines title "E'",\
	fileName using 1:7 with lines title "E''",\
	fileName using 1:8 with lines title "E'lin"
