######################################
#
# midi-switchbox3
#
######################################

# where to find the source code - locally in this case
MIDI_SWITCHBOX3_SITE_METHOD = local
MIDI_SWITCHBOX3_SITE = $($(PKG)_PKGDIR)/

# even though this is a local build, we still need a version number
# bump this number if you need to force a rebuild
MIDI_SWITCHBOX3_VERSION = 1

# dependencies (list of other buildroot packages, separated by space)
MIDI_SWITCHBOX3_DEPENDENCIES =

# LV2 bundles that this package generates (space separated list)
MIDI_SWITCHBOX3_BUNDLES = midi-switchbox3.lv2

# call make with the current arguments and path. "$(@D)" is the build directory.
MIDI_SWITCHBOX3_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D)/source


# build command
define MIDI_SWITCHBOX3_BUILD_CMDS
	$(MIDI_SWITCHBOX3_TARGET_MAKE)
endef

# install command
define MIDI_SWITCHBOX3_INSTALL_TARGET_CMDS
	$(MIDI_SWITCHBOX3_TARGET_MAKE) install DESTDIR=$(TARGET_DIR)
endef


# import everything else from the buildroot generic package
$(eval $(generic-package))
