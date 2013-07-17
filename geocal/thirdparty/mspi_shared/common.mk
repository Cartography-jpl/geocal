# Get list of source files.
local_source := $(wildcard $(local_dir)/src/*.cc)

# Generate list of dependency files, including unit-tests.
local_dep := $(patsubst $(local_dir)/src/%.cc, $(local_dir)/dep/%.d, $(local_source))

# Generate list of object files, including unit-tests.
local_obj := $(patsubst $(local_dir)/src/%.cc, $(local_dir)/obj/%.o, $(local_source))

# Generate list of object files, excluding unit-tests and executables.
local_libobj := $(filter-out %_main.o, $(filter-out %_test.o, $(local_obj)))

# Generate list of unit-tests and outputs
local_unit_test_source := $(filter %_test.cc, $(local_source))
local_unit_test := $(patsubst $(local_dir)/src/%_test.cc, $(local_dir)/unit_test/%_test, $(local_unit_test_source))
local_test_out := $(patsubst $(local_dir)/unit_test/%_test, $(local_dir)/testout/%_test.out, $(local_unit_test))

# Generate list of main exectables
local_exec_source := $(filter %_main.cc, $(local_source))
local_exec := $(patsubst $(local_dir)/src/%_main.cc, $(local_dir)/bin/%_main, $(local_exec_source))

# Create subdirectories needed for build and unit-tests

$(eval $(call mkdir,$(local_dir)/obj))
$(eval $(call mkdir,$(local_dir)/dep))
$(eval $(call mkdir,$(local_dir)/unit_test))
$(eval $(call mkdir,$(local_dir)/bin))
$(eval $(call mkdir,$(local_dir)/testout))
$(eval $(call mkdir,$(local_dir)/tmp))

# Generate build rules
# IMPORTANT: Remove all extraneous white-space from argument lists to 'call'

$(eval $(call make-dependency,$(local_dir),$(local_dep)))
$(eval $(call make-object,$(local_dir),$(local_obj)))
$(eval $(call append-libobj,$(local_libobj)))
$(eval $(call make-unit-test,$(local_dir),$(local_unit_test)))
$(eval $(call make-exec,$(local_dir),$(local_exec)))
$(eval $(call make-test-out,$(local_dir),$(local_test_out)))
