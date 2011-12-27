threads=8
n=1
all: 0wnon 0w 1w 2w 1r 
ARCH=$(shell python -c "import platform; print platform.architecture()[0][0:2]")
DATE=$(shell date +"%T_%b_%d_%Y")
OUTDIR=out

1w:   outdir 
	./testrunner.sh $(threads) $(n) $(OUTDIR) rwl v 1 | tee $(OUTDIR)/testsummary_micro_rwl_vr_1w.out

2w:  outdir
	./testrunner.sh $(threads) $(n) $(OUTDIR) rwl v 2 | tee $(OUTDIR)/testsummary_micro_rwl_vr_2w.out

1r:  outdir
	./testrunner.sh $(threads) $(n) $(OUTDIR) rwl 1 v | tee $(OUTDIR)/testsummary_micro_rwl_1r_vw.out

0wnon: outdir
	./testrunner.sh $(threads) $(n) $(OUTDIR) non v 0 | tee $(OUTDIR)/testsummary_micro_non_vr_0w.out

0w: outdir
	./testrunner.sh $(threads) $(n) $(OUTDIR) rwl v 0 | tee $(OUTDIR)/testsummary_micro_rwl_vr_0w.out

outdir:
	mkdir -p $(OUTDIR) >@

mean: mean.c
	gcc -o $@ -lm $< 	


clean:
	rm -rf mean
	rm -rf *.out
