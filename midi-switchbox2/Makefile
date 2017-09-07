#!/usr/bin/make -f
# Makefile for midiswitchbox.lv2 #
# --------------------------------- #
# Created by falkTX
#

include Makefile.mk

# --------------------------------------------------------------

PREFIX  ?= /usr/local
DESTDIR ?=

# --------------------------------------------------------------
# Default target is to build all plugins

all: build
build: midiswitchbox-build

# --------------------------------------------------------------
# midiswitchbox build rules

midiswitchbox-build: midiswitchbox.lv2/midiswitchbox$(LIB_EXT) midiswitchbox.lv2/manifest.ttl

midiswitchbox.lv2/midiswitchbox$(LIB_EXT): midiswitchbox.c
	$(CC) $^ $(BUILD_C_FLAGS) $(LINK_FLAGS) -lm $(SHARED) -o $@

midiswitchbox.lv2/manifest.ttl: midiswitchbox.lv2/manifest.ttl
	sed -e "s|@LIB_EXT@|$(LIB_EXT)|" $< > $@

# --------------------------------------------------------------

clean:
	rm -f midiswitchbox.lv2/midiswitchbox$(LIB_EXT) midiswitchbox.lv2/manifest.ttl

# --------------------------------------------------------------

install: build
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/midiswitchbox.lv2

	install -m 644 midiswitchbox.lv2/*.so  $(DESTDIR)$(PREFIX)/lib/lv2/midiswitchbox.lv2/
	install -m 644 midiswitchbox.lv2/*.ttl $(DESTDIR)$(PREFIX)/lib/lv2/midiswitchbox.lv2/

# --------------------------------------------------------------
