
PREFIX ?=
DESTDIR =

all: plugins

plugins:
	$(MAKE) -C midi-switchbox_1-2.lv2
	$(MAKE) -C midi-switchbox_1-3.lv2
	$(MAKE) -C midi-switchbox_2-1.lv2
	$(MAKE) -C midi-switchbox_3-1.lv2
	$(MAKE) -C midi-switchbox_1-2_2C.lv2
	$(MAKE) -C midi-switchbox_2-1_2C.lv2
	$(MAKE) -C peak-to-cc.lv2

install:
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox_1-2.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox_1-3.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox_2-1.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox_3-1.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox_1-2_2C.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C midi-switchbox_2-1_2C.lv2
	$(MAKE) install PREFIX=$(PREFIX) -C peak-to-cc.lv2

clean:
	$(MAKE) clean -C midi-switchbox_1-2.lv2
	$(MAKE) clean -C midi-switchbox_1-3.lv2
	$(MAKE) clean -C midi-switchbox_2-1.lv2
	$(MAKE) clean -C midi-switchbox_3-1.lv2
	$(MAKE) clean -C midi-switchbox_1-2_2C.lv2
	$(MAKE) clean -C midi-switchbox_2-1_2C.lv2
	$(MAKE) clean -C peak-to-cc.lv2
