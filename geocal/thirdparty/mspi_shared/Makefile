####################################################################
# Set TOP_DIR
####################################################################

TOP_DIR := $(shell cd ..; pwd)
export TOP_DIR

####################################################################
# Debug option flag
####################################################################

DEBUGFLAG :=
OPTFLAG := -O2

ifeq ($(DEBUG),t)
  DEBUGFLAG := -g
  OPTFLAG :=
endif
ifeq ($(DEBUG_OPT),t)
  DEBUGFLAG := -g
  OPTFLAG := -O2
endif

####################################################################
# Collect information from each module in these four variables.
# Initialize them here as simple variables.
####################################################################

HDF4INC=/usr/include/hdf
HDF4LIB=/usr/lib64/hdf
HDFEOS2INC=/opt/afids_support/include/hdfeos
HDFEOS2LIB=/opt/afids_support/lib
HDFEOS5INC=/data/svn_workspace/mikeb/sreca/mspi/hdfeos5/hdfeos5/include
HDFEOS5LIB=/data/svn_workspace/mikeb/sreca/mspi/hdfeos5/hdfeos5/hdfeos5/lib
HDF5INC=
HDF5LIB=
HDFLIBS = -L${HDFEOS5LIB} -lhe5_hdfeos -lhdf5 -lhdf5_hl -lhdf5_cpp -L$(HDF4LIB) -L$(HDFEOS2LIB) -lhdfeos -lGctp -lmfhdf -ldf -ljpeg -lz -L$(SZIPLIB) -lsz
LOCAL_LIB=libMSPI-Shared.a
OTHERLIBS=-lboost_regex -lboost_filesystem -lboost_system -lboost_date_time -lgdal
CXXFLAGS += -Wall -Werror

####################################################################
# Macro definition
# $(call mkdir, dir)
####################################################################

define mkdir
  ifeq ($(wildcard $1/),)
     $(shell mkdir -p $1)
  endif
endef

####################################################################
# Macro definition
# $(call source-to-object, source-file-list)
####################################################################

source-to-object = $(patsubst $1/src/%.cc,$1/obj/%.o,\
                      $(filter $1/src/%.cc,$2)) \
	           $(patsubst $1/src/%.c,$1/obj/%.o,\
                      $(filter $1/src/%.c,$2))

####################################################################
# Macro definition
# $(stubdirectory)
####################################################################

stubdirectory = $(patsubst %/stub.mk,%,                        \
                 $(word                                         \
                   $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

####################################################################
# Macro definition
# $(call append-libobj, $1=obj-list)
####################################################################

define append-libobj
  libobjects += $1
endef

####################################################################
# Macro definition
# $(call make-object, $1=subdirectory, $2=object-list)
####################################################################

define make-object
  objects += $2

  $1/obj/%.o: $1/src/%.cc
	$(CXX) -c $(CXXFLAGS) -o $$@ $$<

  $1/obj/%.o: $1/src/%.c
	$(CC) -c $(CFLAGS) -o $$@ $$<
endef

####################################################################
# Macro definition
# $(call make-dependency, $1=subdirectory, $2=dependency-list)
####################################################################

define make-dependency
  dependencies += $2

  $1/dep/%.d: $1/src/%.cc
	$(CXX) -c $(CXXFLAGS) $(TARGET_ARCH) -M $$< | \
	$(SED) 's,\($$*\.o\) *:,$1/obj/\1 $$@: ,' > $$@.tmp
	$(MV) $$@.tmp $$@

  $1/dep/%.d: $1/src/%.c
	$(CC) -c $(CFLAGS) $(TARGET_ARCH) -M $$< | \
	$(SED) 's,\($$*\.o\) *:,$1/obj/\1 $$@: ,' > $$@.tmp
	$(MV) $$@.tmp $$@
endef

####################################################################
# Macro definition
# $(call make-unit-test, $1=subdirectory, $2=unit-test-list)
####################################################################

define make-unit-test
  unit_test_execs += $2

  $1/unit_test/%_test : $1/obj/%_test.o $(LOCAL_LIB)
	$(CXX) $(LDFLAGS) -o $$@  $$< $(LOCAL_LIB) $(HDFLIBS) $(OTHERLIBS)
endef

####################################################################
# Macro definition
# $(call make-exec, $1=subdirectory, $2=exec-list)
####################################################################

define make-exec
  execs += $2

  $1/bin/% : $1/obj/%.o $(LOCAL_LIB)
	$(CXX) $(LDFLAGS) -o $$@  $$< $(LOCAL_LIB) $(HDFLIBS) $(PGSTKLIBS) $(OTHERLIBS)
endef

####################################################################
# Macro definition
# $(call make-test-out, $1=subdirectory, $2=test-out-list)
####################################################################

define make-test-out
  unit_test_outs += $2

  $1/testout/%_test.out : $1/unit_test/%_test
	$$^ 1> $$@ 2>&1

endef

####################################################################
# Collect information from each module in these four variables.
# Initialize them here as simple variables.
####################################################################

modules := ErrorHandling 
modules += Config 
modules += UnitTest
modules += Support 
modules += File/L1AFile 
modules += File/L1B2File
modules += File/Table 
modules += File/L1B1File
modules += File/FieldMap
modules += File/hdf4/L1AFile 
modules += File/hdf4/L1B2File
modules += File/hdf4/Table
modules += File/hdf4/L1B1File
modules += File/Geotiff
modules += File/Ancillary
modules += File/Meta
modules += File/FileHandle
tmp_dirs     += $(addsuffix /tmp, $(modules))
stubs        := $(addsuffix /stub.mk,$(modules))
unit_test_execs :=
objects      :=
libobjects   :=
dependencies :=
unit_test_outs :=

include_dirs := .  $(HDFEOS2INC) $(HDFEOS5INC) $(HDF4INC) $(HDF5INC) $(TOP_DIR)

INCFLAGS  = $(addprefix -I ,$(include_dirs))

CXXFLAGS+= $(DEBUGFLAG) $(OPTFLAG) $(INCFLAGS)
CFLAGS+= $(DEBUGFLAG) $(OPTFLAG) $(INCFLAGS)
LDFLAGS+= -rdynamic

vpath %.h $(include_dirs)

MV  := mv -f
RM  := rm -f
RMDIR  := rmdir
SED := sed
DOXYGEN := doxygen
COMMON_MK := $(TOP_DIR)/MSPI-Shared/common.mk
STUB_MK := $(TOP_DIR)/MSPI-Shared/stub.mk

all:

%/stub.mk: 
	ln -s $(STUB_MK) $@

-include $(stubs)

ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

$(LOCAL_LIB): $(libobjects) $(stubs)
	$(AR) $(ARFLAGS) $@ $(libobjects)

.PHONY: all
all: $(LOCAL_LIB) $(execs)

.PHONY: test unit_test
test unit_test : $(unit_test_outs)
	@python check.py $(unit_test_outs)

.PHONY: doc

doc:
	$(DOXYGEN) doxygen.config


.SECONDARY: $(unit_test_execs) 

.PHONY: clean

clean:
	$(RM) $(objects) $(execs) $(LOCAL_LIB) \
              $(dependencies) $(unit_tests) $(test_outs) $(unit_test_execs) $(stubs)
	$(RM) -r $(tmp_dirs)


