include ../Makefile.mk

all: build

build: midi-switchbox.so

midi-switchbox.so: midi-switchbox.c.o
	$(CC) $^ $(LDFLAGS) -shared -Wl,--no-undefined -o $@

midi-switchbox.c.o: midi-switchbox.c
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -f *.o *.so
