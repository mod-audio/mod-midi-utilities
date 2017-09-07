#!/usr/bin/make -f
# Makefile for midi-switchbox4.lv2 #
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
build: midi-switchbox4-build

# --------------------------------------------------------------
# midi-switchbox4 build rules

midi-switchbox4-build: midi-switchbox4.lv2/midi-switchbox4$(LIB_EXT) midi-switchbox4.lv2/manifest.ttl
midi-switchbox4.lv2/midi-switchbox4$(LIB_EXT): midi-switchbox4.c
	$(CC) $^ $(BUILD_C_FLAGS) $(LINK_FLAGS) -lm $(SHARED) -o $@

midi-switchbox4.lv2/manifest.ttl: midi-switchbox4.lv2/manifest.ttl
	sed -e "s|@LIB_EXT@|$(LIB_EXT)|" $< > $@

# --------------------------------------------------------------

clean:
	rm -f midi-switchbox4.lv2/midi-switchbox4$(LIB_EXT) midi-switchbox4.lv2/manifest.ttl

# --------------------------------------------------------------

install: build
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/midi-switchbox4.lv2

	install -m 644 midi-switchbox4.lv2/*.so  $(DESTDIR)$(PREFIX)/lib/lv2/midi-switchbox4.lv2/
	install -m 644 midi-switchbox4.lv2/*.ttl $(DESTDIR)$(PREFIX)/lib/lv2/midi-switchbox4.lv2/

# --------------------------------------------------------------
