# Retrieve subdirectory name
local_dir := $(stubdirectory)

# Load extra rules for this module (if available)
-include $(local_dir)/module.mk

# Load common makefile
include $(COMMON_MK)
