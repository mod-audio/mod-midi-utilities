
PREFIX ?= /usr/local
DESTDIR =

all: plugins

plugins:
	$(MAKE) -C midi-inv-switchbox.lv2
	$(MAKE) -C midi-switchbox.lv2
	$(MAKE) -C midi-switchbox3.lv2
	$(MAKE) -C midi-switchbox4.lv2
	$(MAKE) -C peak-to-cc.lv2

install:
	$(MAKE) install PREFIX=$(PREFIX) -C midi-inv-switchbox.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox3.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox4.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C peak-to-cc.lv2

clean:
	$(MAKE) clean -C midi-inv-switchbox.lv2
	$(MAKE) clean -C midi-switchbox.lv2
	$(MAKE) clean -C midi-switchbox3.lv2
	$(MAKE) clean -C midi-switchbox4.lv2
	$(MAKE) clean -C peak-to-cc.lv2
