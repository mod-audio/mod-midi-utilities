include ../Makefile.mk

NAME = midi-clock-info

all: build
build: $(NAME).so

$(NAME).so: $(NAME).c.o
	$(CXX) $^ $(LDFLAGS) -shared -Wl,--no-undefined -o $@

$(NAME).c.o: $(NAME).c
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -f *.o *.so

install: build
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/$(NAME).lv2

	install -m 644 *.so  $(DESTDIR)$(PREFIX)/lib/lv2/$(NAME).lv2/
	install -m 644 *.ttl $(DESTDIR)$(PREFIX)/lib/lv2/$(NAME).lv2/
