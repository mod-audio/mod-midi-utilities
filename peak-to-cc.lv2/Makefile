include ../Makefile.mk

NAME = peak-to-cc

all: build
build: $(NAME).so

$(NAME).so: $(NAME).cpp.o
	$(CXX) $^ $(LDFLAGS) -shared -Wl,--no-undefined -o $@

$(NAME).cpp.o: $(NAME).cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

clean:
	rm -f *.o *.so

install: build
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/$(NAME).lv2

	install -m 644 *.so  $(DESTDIR)$(PREFIX)/lib/lv2/$(NAME).lv2/
	install -m 644 *.ttl $(DESTDIR)$(PREFIX)/lib/lv2/$(NAME).lv2/
