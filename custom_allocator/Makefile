CPPFLAGS=-g -Wall
SRC=allocchunk.cc allocchunk.hh testallocchunk.cc
all: testallocchunk


testallocchunk: $(SRC)
	$(CXX) $(CPPFLAGS) $(SRC) -o $@

clean:
	rm -rf testallocchunk

.PHONY: clean