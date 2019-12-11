
PREFIX ?= /usr/local
DESTDIR =

all: plugins

plugins:
	$(MAKE) -C midi-switchbox-1inx2out.lv2
	$(MAKE) -C midi-switchbox-1inx3out.lv2
	$(MAKE) -C midi-switchbox-2inx1out.lv2
	$(MAKE) -C midi-switchbox-3inx1out.lv2
	$(MAKE) -C midi-switchbox-2inx4out.lv2
	$(MAKE) -C midi-switchbox-4inx2out.lv2
	$(MAKE) -C peak-to-cc.lv2

install:
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox-1inx2out.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox-1inx3out.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox-2inx1out.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox-3inx1out.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox-2inx4out.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox-4inx2out.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C peak-to-cc.lv2

clean:
	$(MAKE) clean -C midi-switchbox-1inx2out.lv2
	$(MAKE) clean -C midi-switchbox-1inx3out.lv2
	$(MAKE) clean -C midi-switchbox-2inx1out.lv2
	$(MAKE) clean -C midi-switchbox-3inx1out.lv2
	$(MAKE) clean -C midi-switchbox-2inx4out.lv2
	$(MAKE) clean -C midi-switchbox-4inx2out.lv2
	$(MAKE) clean -C peak-to-cc.lv2
