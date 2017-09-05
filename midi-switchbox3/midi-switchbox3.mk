######################################
#
# midiswitchbox2
#
######################################

# where to find the source code - locally in this case
MIDISWITCHBOX2_SITE_METHOD = local
MIDISWITCHBOX2_SITE = $($(PKG)_PKGDIR)/

# even though this is a local build, we still need a version number
# bump this number if you need to force a rebuild
MIDISWITCHBOX2_VERSION = 1

# dependencies (list of other buildroot packages, separated by space)
MIDISWITCHBOX2_DEPENDENCIES =

# LV2 bundles that this package generates (space separated list)
MIDISWITCHBOX2_BUNDLES = midiswitchbox2.lv2

# call make with the current arguments and path. "$(@D)" is the build directory.
MIDISWITCHBOX2_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D)/source


# build command
define MIDISWITCHBOX2_BUILD_CMDS
	$(MIDISWITCHBOX2_TARGET_MAKE)
endef

# install command
define MIDISWITCHBOX2_INSTALL_TARGET_CMDS
	$(MIDISWITCHBOX2_TARGET_MAKE) install DESTDIR=$(TARGET_DIR)
endef


# import everything else from the buildroot generic package
$(eval $(generic-package))
