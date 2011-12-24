threads=8
n=1
all: macro
ARCH=$(shell python -c "import platform; print platform.architecture()[0][0:2]")
DATE=$(shell date +"%T_%b_%d_%Y")
OUTDIR=out
macro: 
	mkdir -p $(OUTDIR) 
	./testrunner.sh $(threads) $(n) $(OUTDIR) | tee $(OUTDIR)/testsummary_rwlnon_$(DATE).out