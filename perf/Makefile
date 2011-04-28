conf=compare_all.conf
n=10
ARCH=$(shell python -c "import platform; print platform.architecture()[0]")
DATE=$(shell date +"%b.%d.%Y")

all: 366k
.PHONY: 167k 1m_gt20 1m_32 366k

167k 1m_gt20 366k 1m_20:
	./testrunner.sh $(n) $@ $(conf) 5mtest.dump | tee means.$@.$(ARCH).$(DATE).out

1m_32:
	./testrunner.sh $(n) $@ $(conf) 5mshuffle.dump | tee means.$@.$(ARCH).$(DATE).out




