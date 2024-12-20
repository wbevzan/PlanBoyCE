# ----------------------------
# Makefile Options
# ----------------------------

# TODO change this stuffs
NAME = DEMO
ICON = icon.png
DESCRIPTION = "PlanBoy CE"
COMPRESSED = YES
LTO = YES
ARCHIVED = YES
COMPRESSED_MODE = zx0 #This is awesome
PREFER_OS_LIBC = YES
HAS_PRINTF = NO
# FAILING_SCIENCE_FAIR = YES
CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------
include $(shell cedev-config --makefile)
