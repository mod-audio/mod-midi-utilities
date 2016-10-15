include ../Makefile.mk

all: build

build: peak-to-cc.so

peak-to-cc.so: peak-to-cc.cpp.o
	$(CXX) $^ $(LDFLAGS) -shared -Wl,--no-undefined -o $@

peak-to-cc.cpp.o: peak-to-cc.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

clean:
	rm -f *.o *.so
