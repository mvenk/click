all: report.pdf

report.pdf: report.tex
	pdflatex $<
	pdflatex $<

view:
	evince report.pdf &

clean:
	rm -rf *.aux *.log *.pdf

.PHONY: clean