n=1
all: macro
ARCH=$(shell python -c "import platform; print platform.architecture()[0][0:2]")
DATE=$(shell date +"%b_%d_%Y")
OUTDIR=out
macro: mean
	mkdir -p $(OUTDIR) 
	./testrunner.sh $(n) $(OUTDIR) | tee $(OUTDIR)/testsummary_$(DATE).out

mean:
	cd ../;make mean
