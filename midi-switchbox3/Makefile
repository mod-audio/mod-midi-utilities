#!/usr/bin/make -f
# Makefile for midi-switchbox3.lv2 #
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
build: midi-switchbox3-build

# --------------------------------------------------------------
# midi-switchbox3 build rules

midi-switchbox3-build: midi-switchbox3.lv2/midi-switchbox3$(LIB_EXT) midi-switchbox3.lv2/manifest.ttl
midi-switchbox3.lv2/midi-switchbox3$(LIB_EXT): midi-switchbox3.c
	$(CC) $^ $(BUILD_C_FLAGS) $(LINK_FLAGS) -lm $(SHARED) -o $@

midi-switchbox3.lv2/manifest.ttl: midi-switchbox3.lv2/manifest.ttl
	sed -e "s|@LIB_EXT@|$(LIB_EXT)|" $< > $@

# --------------------------------------------------------------

clean:
	rm -f midi-switchbox3.lv2/midi-switchbox3$(LIB_EXT) midi-switchbox3.lv2/manifest.ttl

# --------------------------------------------------------------

install: build
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/midi-switchbox3.lv2

	install -m 644 midi-switchbox3.lv2/*.so  $(DESTDIR)$(PREFIX)/lib/lv2/midi-switchbox3.lv2/
	install -m 644 midi-switchbox3.lv2/*.ttl $(DESTDIR)$(PREFIX)/lib/lv2/midi-switchbox3.lv2/

# --------------------------------------------------------------
