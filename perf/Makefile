conf=compare_all.conf
n=10
ARCH=$(shell python -c "import platform; print platform.architecture()[0]")
DATE=$(shell date +"%b.%d.%Y")

all: 167k 1m_gt20 1m_32 366k 1m_20
.PHONY: 167k 1m_gt20 1m_32 366k 1m_20

167k 1m_gt20 366k 1m_20:
	mkdir -p out/$(conf)
	./testrunner.sh $(n) $@ $(conf) 5mtest.dump | tee out/$(conf)/means.$@.$(ARCH).$(DATE).out

1m_32:
	mkdir -p $(conf)
	./testrunner.sh $(n) $@ $(conf) 5mshuffle.dump | tee out/$(conf)/means.$@.$(ARCH).$(DATE).out


cache: 167kc 1m_gt20c 1m_32c 366kc 1m_20c
.PHONY: 167kc 1m_gt20c 1m_32c 366kc 1m_20c

167kc 1m_gt20c 366kc 1m_20c:
	./cacherunner.sh `echo $@ | sed -e "s#c##"` $(conf) 5mtest.dump

1m_32c:
	./cacherunner.sh `echo $@ | sed -e "s#c##"` $(conf) 5mshuffle.dump

