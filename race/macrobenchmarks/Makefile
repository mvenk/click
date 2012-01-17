threads=8
n=4
all: 0w 1w 1r 0r
ARCH=$(shell python -c "import platform; print platform.architecture()[0][0:2]")
DATE=$(shell date +"%T_%b_%d_%Y")
OUTDIR=out

1w:   outdir 
	./testrunner.sh $(threads) $(n) $(OUTDIR) rcu v 1 | tee $(OUTDIR)/testsummary_macro_rcu_vr_1w.out

0w:  outdir
	./testrunner.sh $(threads) $(n) $(OUTDIR) rcu v 0 | tee $(OUTDIR)/testsummary_macro_rcu_vr_0w.out

1r:  outdir
	./testrunner.sh $(threads) $(n) $(OUTDIR) rcu 1 v | tee $(OUTDIR)/testsummary_macro_rcu_1r_vw.out

0r:  outdir
	./testrunner.sh $(threads) $(n) $(OUTDIR) rcu 0 v | tee $(OUTDIR)/testsummary_macro_rcu_1r_vw.\out

profile1w:  outdir
	./profilerunner.sh $(threads) $(n) $(OUTDIR) rcu 1 v

outdir:
	mkdir -p $(OUTDIR) >@

mean: mean.c
	gcc -o $@ -lm $< 	


clean:
	rm -rf mean
	rm -rf *.out
