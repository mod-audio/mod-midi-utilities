
all: plugins

plugins:
	$(MAKE) -C midi-switchbox.lv2

install:
	install -d $(DESTDIR)/usr/lib/lv2/midi-switchbox.lv2
	cp midi-switchbox.lv2/*.ttl $(DESTDIR)/usr/lib/lv2/midi-switchbox.lv2/
	cp midi-switchbox.lv2/*.so  $(DESTDIR)/usr/lib/lv2/midi-switchbox.lv2/
	cp -r midi-switchbox.lv2/modgui  $(DESTDIR)/usr/lib/lv2/midi-switchbox.lv2/

clean:
	$(MAKE) clean -C midi-switchbox.lv2
