threads=8
n=1
all: micro
ARCH=$(shell python -c "import platform; print platform.architecture()[0][0:2]")
DATE=$(shell date +"%T_%b_%d_%Y")
OUTDIR=out
micro: 
	mkdir -p $(OUTDIR) 
	./testrunner.sh $(threads) $(n) $(OUTDIR) | tee $(OUTDIR)/testsummary_rcu_$(DATE).out

mean: mean.c
	gcc -o $@ -lm $< 	


clean:
	rm -rf mean
	rm -rf *.out
