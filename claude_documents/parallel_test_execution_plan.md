# Plan: Parallelizing GeoCal Unit Tests

**Date:** 2026-06-20  
**Author:** Claude Code (Sonnet 4.5)  
**Status:** Planning - Not Yet Implemented

## Executive Summary

GeoCal currently uses a single monolithic test executable (`geocal_test_all`) containing 559 test cases from 178 test files. This document outlines five approaches for parallelizing test execution, ranging from minimal-change solutions to complete build system rewrites. **Approach 1 (Automake Parallel Tests)** is recommended as the quick-win solution requiring only a few hours of work.

---

## Current State Analysis

### Test Structure
- **Single monolithic executable:** `geocal_test_all`
- **178 test files** across the codebase
- **177 test suites** (shown by `--list_content`)
- **559 test cases** total
- **Current runtime:** ~49 seconds (real), ~5m45s (user) - suggesting some internal parallelism already exists

### Test Organization
```
lib/
├── GeoCalCore/
│   ├── Support/       23 test files
│   ├── Interface/     24 test files
│   └── Implementation/ 91 test files
├── CartLab/          ~20 test files
├── Mspi/             ~10 test files
└── VicarImageLib/    ~10 test files
```

### Build System
- Uses autotools (automake/autoconf)
- Single `check_PROGRAMS = geocal_test_all` in `geocal.am`
- All test sources added to `geocal_test_all_SOURCES` via `implementation.am`, `interface.am`, etc.
- Uses **Boost.Test 1.84.0** (modern, with many parallelization features)
- Test runner: `lib/test_all.sh` wrapper script

### Key Files
- **Test entry point:** `lib/test_all.cc` - Defines `BOOST_TEST_MODULE`
- **Build configuration:** `geocal.am` - Defines test program and execution
- **Test organization:** `lib/GeoCalCore/*/implementation.am` - Lists test files
- **Test runner:** `lib/test_all.sh` - Shell wrapper for test execution

---

## Available Approaches (Ranked by Effort)

### Approach 1: Automake Parallel Test Execution ⭐ **RECOMMENDED**

**Effort:** Minimal (few hours)  
**Impact:** Moderate speedup (2-4x depending on test distribution)  
**Stability:** Very stable - uses standard automake features  
**Reversibility:** Easy to revert

#### What It Does
Use automake's built-in parallel test harness (`TESTS` variable) to run multiple test suites from the same executable in parallel.

#### How It Works
1. Keep single `geocal_test_all` executable (no changes to compilation)
2. Create multiple test runner scripts, each invoking different test suites
3. Use automake's `TESTS` variable to run them in parallel with `make -j`

#### Implementation Steps

**Step 1:** Create test runner scripts (one per major component)

Create `lib/GeoCalCore/Support/support_tests.sh`:
```bash
#!/bin/bash
# Run all Support module tests
cd $(dirname $0)/../../..
./geocal_test_all --run_test=geocal_exception,geocal_thread_pool,geocal_temp_file,auto_derivative,array_ad,observer,geocal_time,printable,ostream_pad,tile,tiled_file,memory_map_array,geocal_gsl_matrix,geocal_matrix,geocal_quaternion,geocal_gsl_fit,geocal_gsl_root,covariance
```

Create `lib/GeoCalCore/Interface/interface_tests.sh`:
```bash
#!/bin/bash
# Run all Interface module tests
cd $(dirname $0)/../../..
./geocal_test_all --run_test=orbit,camera,coordinate_converter,image_coordinate,frame_coordinate,ground_coordinate,look_vector,image_ground_connection,dem,raster_image,map_info,simple_dem,igc_collection,tie_point_collection,simultaneous_bundle_adjustment
```

Create `lib/GeoCalCore/Implementation/implementation_tests_1.sh`:
```bash
#!/bin/bash
# Run Implementation tests (group 1: orbit-related)
cd $(dirname $0)/../../..
./geocal_test_all --run_test=orbit_quaternion_list,combine_orbit,orbit_des,orbit_offset_correction,orbit_piecewise_correction,orbit_time_shift,tle_orbit,spice_orbit,orbit_data_image_ground_connection,aircraft_orbit_data,hdf_orbit
```

Create `lib/GeoCalCore/Implementation/implementation_tests_2.sh`:
```bash
#!/bin/bash
# Run Implementation tests (group 2: image/camera-related)
cd $(dirname $0)/../../..
./geocal_test_all --run_test=raster_averaged,raster_subsample,magnify_bilinear,smooth_image,pan_sharpen,quaternion_camera,camera_rational_polynomial,camera_radial_distortion,galileo_camera,sensrb_camera,glas_gfm_camera
```

Create `lib/GeoCalCore/Implementation/implementation_tests_3.sh`:
```bash
#!/bin/bash
# Run Implementation tests (group 3: IGC/matching-related)
cd $(dirname $0)/../../..
./geocal_test_all --run_test=igc_image_to_image_match,surface_image_to_image_match,dem_match,phase_correlation_matcher,igc_ray_caster,igc_rolling_shutter,igc_array,igc_multiple_pass,igc_collection_rolling_shutter,igc_collection_orbit_data
```

Create `lib/GeoCalCore/Implementation/implementation_tests_4.sh`:
```bash
#!/bin/bash
# Run Implementation tests (group 4: RSM/other)
cd $(dirname $0)/../../..
./geocal_test_all --run_test=rsm_polynomial,rsm_grid,rsm_rp_plus_grid,rsm_low_order_polynomial,rsm_id,rsm_direct_covariance,rsm_direct_covariance_a,rsm_indirect_covariance,rsm_indirect_covariance_b,rsm_adjustable_parameter_a,rsm_adjustable_parameter_b,rsm_multi_section,rsm_test,rsm_nitf,rsm_image_ground_connection
```

Create `lib/CartLab/cartlab_tests.sh`:
```bash
#!/bin/bash
# Run all CartLab module tests
cd $(dirname $0)/../..
./geocal_test_all --run_test=argus_camera,argus_orbit,spot_orbit,spot_camera,quickbird_camera,quickbird_orbit,quickbird_time_table
```

Create `lib/Mspi/mspi_tests.sh`:
```bash
#!/bin/bash
# Run all MSPI module tests
cd $(dirname $0)/../..
./geocal_test_all --run_test=air_mspi_orbit,ground_mspi_orbit
```

**Step 2:** Modify `geocal.am`:
```makefile
# Replace:
# TESTS+= @libgeocal@/test_all.sh

# With:
TESTS = lib/GeoCalCore/Support/support_tests.sh \
        lib/GeoCalCore/Interface/interface_tests.sh \
        lib/GeoCalCore/Implementation/implementation_tests_1.sh \
        lib/GeoCalCore/Implementation/implementation_tests_2.sh \
        lib/GeoCalCore/Implementation/implementation_tests_3.sh \
        lib/GeoCalCore/Implementation/implementation_tests_4.sh \
        lib/CartLab/cartlab_tests.sh \
        lib/Mspi/mspi_tests.sh

# Make scripts executable
dist_check_SCRIPTS = $(TESTS)
```

**Step 3:** Make scripts executable:
```bash
chmod +x lib/GeoCalCore/Support/support_tests.sh
chmod +x lib/GeoCalCore/Interface/interface_tests.sh
chmod +x lib/GeoCalCore/Implementation/implementation_tests_*.sh
chmod +x lib/CartLab/cartlab_tests.sh
chmod +x lib/Mspi/mspi_tests.sh
```

**Step 4:** Run tests in parallel:
```bash
make check -j20  # Runs up to 20 test scripts in parallel
```

#### Discovering Test Suite Names

To get the list of test suites for organizing into groups:
```bash
./geocal_test_all --list_content | grep "^\S" | cut -d'*' -f1
```

To test a specific group:
```bash
./geocal_test_all --run_test=combine_orbit,orbit_quaternion_list
```

#### Pros
- ✅ Minimal code changes
- ✅ Uses standard automake features
- ✅ Easy to add/remove test groups
- ✅ No changes to test code itself
- ✅ Compatible with existing infrastructure
- ✅ Easy to revert if issues arise
- ✅ Can keep `lib/test_all.sh` as fallback

#### Cons
- ⚠️ Requires manual organization of tests into groups
- ⚠️ All tests still linked into one large executable
- ⚠️ Need to maintain test group organization as tests are added

---

### Approach 2: Multiple Test Executables by Module

**Effort:** Moderate (1-2 days)  
**Impact:** Good speedup (3-6x) + better compile times  
**Stability:** Stable - standard practice in many projects

#### What It Does
Split `geocal_test_all` into multiple executables, one per major module.

#### How It Works
Each test executable only links the tests for its module, reducing executable size and allowing true parallel execution.

#### Implementation Steps

**Step 1:** Define multiple test programs in `geocal.am`:
```makefile
check_PROGRAMS = geocal_support_test \
                 geocal_interface_test \
                 geocal_implementation_test \
                 geocal_cartlab_test \
                 geocal_mspi_test

# Support tests
geocal_support_test_SOURCES = @libgeocal@/test_main.cc
geocal_support_test_LDADD = libgeocal.la $(CARTO_LIBS) $(VICAR_RTL_LIBS) $(SPICE_LIBS)
geocal_support_test_LDADD += $(GDAL_LIBS) $(OPENSSL_LIBS) $(GEOTIFF_LIBS) $(CODE_COVERAGE_LIBS)
geocal_support_test_LDFLAGS = -no-install $(OPENMP_CFLAGS)
geocal_support_test_CPPFLAGS = $(AM_CPPFLAGS) $(CODE_COVERAGE_CPPFLAGS) $(OPENMP_CFLAGS)

# Interface tests
geocal_interface_test_SOURCES = @libgeocal@/test_main.cc
geocal_interface_test_LDADD = libgeocal.la $(CARTO_LIBS) $(VICAR_RTL_LIBS) $(SPICE_LIBS)
geocal_interface_test_LDADD += $(GDAL_LIBS) $(OPENSSL_LIBS) $(GEOTIFF_LIBS) $(CODE_COVERAGE_LIBS)
geocal_interface_test_LDFLAGS = -no-install $(OPENMP_CFLAGS)
geocal_interface_test_CPPFLAGS = $(AM_CPPFLAGS) $(CODE_COVERAGE_CPPFLAGS) $(OPENMP_CFLAGS)

# Implementation tests
geocal_implementation_test_SOURCES = @libgeocal@/test_main.cc
geocal_implementation_test_LDADD = libgeocal.la $(CARTO_LIBS) $(VICAR_RTL_LIBS) $(SPICE_LIBS)
geocal_implementation_test_LDADD += $(GDAL_LIBS) $(OPENSSL_LIBS) $(GEOTIFF_LIBS) $(CODE_COVERAGE_LIBS)
geocal_implementation_test_LDFLAGS = -no-install $(OPENMP_CFLAGS)
geocal_implementation_test_CPPFLAGS = $(AM_CPPFLAGS) $(CODE_COVERAGE_CPPFLAGS) $(OPENMP_CFLAGS)

# Parallel execution
TESTS = $(check_PROGRAMS)
```

**Step 2:** Reorganize test source lists in `implementation.am`, `interface.am`, `support.am`:
```makefile
# In implementation.am
# Replace:
# geocal_test_all_SOURCES += @srcimplementation@/combine_orbit_test.cc

# With:
geocal_implementation_test_SOURCES += @srcimplementation@/combine_orbit_test.cc
```

**Step 3:** Run tests:
```bash
make check -j20  # Compiles 5 test executables and runs them in parallel
```

#### Pros
- ✅ True parallel compilation of test executables
- ✅ Smaller executable sizes (faster linking)
- ✅ Better isolation - one test crash doesn't kill all tests
- ✅ Easier to run specific module tests
- ✅ Still uses standard automake patterns

#### Cons
- ⚠️ More changes to build files
- ⚠️ Need to manage shared fixtures/utilities carefully
- ⚠️ Slightly more complex dependency management
- ⚠️ Each executable pays fixed library linking cost

---

### Approach 3: One Test Executable Per File

**Effort:** High (3-5 days)  
**Impact:** Maximum speedup (10-20x potential) + very fast incremental builds  
**Stability:** Requires more maintenance

#### What It Does
Create one test executable per `*_test.cc` file (178 executables).

#### How It Works
Each test file becomes its own executable, maximizing parallelization.

#### Implementation Overview

**Generate test program entries automatically in `geocal.am`:**
```makefile
# For each test file, generate a program
TEST_SOURCES = $(wildcard lib/GeoCalCore/*_test.cc)
TEST_PROGRAMS = $(TEST_SOURCES:.cc=)

check_PROGRAMS = $(TEST_PROGRAMS)

# Template for each test
define TEST_TEMPLATE
$(1)_SOURCES = $(1).cc @libgeocal@/test_main.cc
$(1)_LDADD = libgeocal.la $$(ALL_LIBS)
$(1)_LDFLAGS = -no-install
endef

$(foreach test,$(TEST_PROGRAMS),$(eval $(call TEST_TEMPLATE,$(test))))

TESTS = $(check_PROGRAMS)
```

**Run with maximum parallelism:**
```bash
make check -j$(nproc)  # Runs all 178 tests in parallel
```

#### Pros
- ✅ Maximum parallelization
- ✅ Very fast incremental rebuilds (only changed tests recompile)
- ✅ Excellent for CI/CD pipelines
- ✅ Best isolation between tests

#### Cons
- ⚠️ 178 executables to manage
- ⚠️ Longer initial full build time (lots of linking)
- ⚠️ More complex makefile generation
- ⚠️ May hit filesystem limits on some systems
- ⚠️ Each executable pays fixed linking cost (~178x overhead)

---

### Approach 4: CTest Integration (Modern Approach)

**Effort:** High (complete build system rewrite)  
**Impact:** Excellent tooling + parallel execution  
**Stability:** Very stable, modern standard

#### What It Does
Replace autotools with CMake/CTest for test management.

#### How It Works
CMake's CTest has built-in parallel test execution and better reporting.

#### Implementation Overview
```cmake
# CMakeLists.txt
enable_testing()

# Auto-discover boost tests and create one CTest per test suite
function(add_geocal_test test_name)
    add_executable(${test_name} ${ARGN})
    target_link_libraries(${test_name} geocal ${Boost_LIBRARIES})
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Add all tests
add_geocal_test(orbit_test orbit_test.cc)
add_geocal_test(camera_test camera_test.cc)
# ... etc ...
```

**Run tests:**
```bash
ctest -j20  # Parallel execution with nice reporting
ctest --output-on-failure  # Better debugging
```

#### Pros
- ✅ Modern, industry-standard approach
- ✅ Excellent parallel execution
- ✅ Great reporting (XML, JSON output)
- ✅ Better integration with CI/CD
- ✅ Test timing statistics built-in
- ✅ Can mix different test executables easily

#### Cons
- ⚠️ Requires rewriting entire build system
- ⚠️ Breaking change for existing workflows
- ⚠️ Learning curve for maintainers
- ⚠️ Out of scope for "minimal changes"
- ⚠️ Would need to port all autotools configuration to CMake

---

### Approach 5: Boost.Test Internal Parallelization

**Effort:** Minimal  
**Impact:** Limited (only helps with certain test types)  
**Stability:** Experimental feature

#### What It Does
Use Boost.Test's experimental parallel execution features (if available in 1.84).

#### Investigation Notes
Boost.Test 1.84 may have experimental parallel execution features, but these are typically:
- Limited to test case parallelization within a single suite
- Not widely used/tested in production
- May have issues with shared fixtures
- Not documented as stable features

**Not recommended** as primary approach, but could complement Approach 1 or 2.

---

## Recommended Implementation Plan

### Phase 1: Quick Win (1-2 hours) ⭐ **START HERE**
**Implement Approach 1 (Automake Parallel Tests):**
1. Get list of test suites: `./geocal_test_all --list_content | grep "^\S"`
2. Create 5-8 test runner scripts grouping tests by module
3. Update `TESTS` variable in `geocal.am`
4. Test with `make check -j20`
5. **Expected speedup: 2-4x**

### Phase 2: If More Speedup Needed (1-2 days)
**Implement Approach 2 (Multiple Executables):**
1. Split into 5-10 test executables by major module
2. Update build files to compile separately
3. Test builds and parallelization
4. **Expected speedup: 4-8x total**

### Phase 3: Future Consideration (when time permits)
**Consider Approach 4 (CTest) as part of larger modernization:**
- Evaluate CMake migration for entire project
- Better CI/CD integration
- Modern tooling benefits

---

## Optimization Tips

### Profiling Tests
Identify slowest test suites to optimize grouping:
```bash
./geocal_test_all --log_level=test_suite --show_progress 2>&1 | grep "test suite"
```

### Grouping Strategy
- **Group fast tests together** - they can share a runner without blocking
- **Isolate slow tests** - give them dedicated runners
- **Balance groups** - aim for similar runtime per group
- **Separate I/O heavy tests** - they may have different bottlenecks

### Monitoring Parallel Execution
```bash
# Watch test processes
watch -n 0.5 'ps aux | grep geocal_test_all | grep -v grep'

# Time each test group
time lib/GeoCalCore/Support/support_tests.sh
```

---

## Testing the Implementation

### Validation Steps

1. **Verify all tests still pass:**
   ```bash
   make check -j20
   ```

2. **Compare with sequential execution:**
   ```bash
   time make check -j1  # Sequential
   time make check -j20 # Parallel
   ```

3. **Check individual test groups:**
   ```bash
   lib/GeoCalCore/Support/support_tests.sh
   lib/GeoCalCore/Interface/interface_tests.sh
   ```

4. **Verify test count is preserved:**
   ```bash
   ./geocal_test_all --list_content | grep "^\S" | wc -l  # Should be 177
   ```

---

## Rollback Plan

If issues arise with Approach 1:

1. **Revert `geocal.am` change:**
   ```makefile
   # Restore:
   TESTS+= @libgeocal@/test_all.sh
   ```

2. **Remove test runner scripts:**
   ```bash
   rm lib/*/*/support_tests.sh
   ```

3. **Rebuild and test:**
   ```bash
   make check
   ```

The existing `lib/test_all.sh` can be kept as a fallback during the transition.

---

## CI/CD Integration

### Update Continuous Integration

For Jenkins/GitLab CI/similar:
```bash
# Instead of:
make check

# Use:
make check -j$(nproc)
```

### Test Artifacts
Each test group can produce separate logs:
```bash
lib/GeoCalCore/Support/support_tests.sh > support_tests.log 2>&1
lib/GeoCalCore/Interface/interface_tests.sh > interface_tests.log 2>&1
```

---

## Maintenance Considerations

### Adding New Tests
When adding a new test file `new_feature_test.cc`:

**Approach 1:** Add to appropriate test group script
```bash
# Edit lib/GeoCalCore/Implementation/implementation_tests_1.sh
# Add new_feature to --run_test= list
```

**Approach 2:** Add to appropriate executable sources
```makefile
# In implementation.am
geocal_implementation_test_SOURCES += @srcimplementation@/new_feature_test.cc
```

### Reorganizing Test Groups
Periodically review test runtimes and rebalance groups:
```bash
# Profile individual test suites
./geocal_test_all --log_level=test_suite --run_test=combine_orbit
```

---

## Performance Expectations

### Current Baseline
- **Single-threaded:** ~49 seconds (real time)
- **User time:** 5m45s (suggests some parallelism exists)
- **Test count:** 559 test cases in 177 suites

### Expected Improvements

**Approach 1 (5-8 parallel groups):**
- **Best case:** 15-20 seconds (3-4x speedup)
- **Typical:** 20-25 seconds (2-3x speedup)
- **Depends on:** Test distribution balance

**Approach 2 (5-10 executables):**
- **Best case:** 10-15 seconds (4-6x speedup)
- **Typical:** 12-18 seconds (3-4x speedup)
- **Depends on:** Compilation parallelism + runtime balance

**Approach 3 (178 executables):**
- **Best case:** 5-10 seconds (6-10x speedup)
- **But:** Initial build much slower due to linking overhead
- **Best for:** Incremental development (only relink changed tests)

---

## References

### Boost.Test Documentation
- **Boost.Test 1.84.0:** https://www.boost.org/doc/libs/1_84_0/libs/test/doc/html/index.html
- **Command-line reference:** `./geocal_test_all --help`
- **Running specific tests:** https://www.boost.org/doc/libs/1_84_0/libs/test/doc/html/boost_test/runtime_config/test_unit_filtering.html

### Automake Documentation
- **Parallel test harness:** https://www.gnu.org/software/automake/manual/html_node/Parallel-Test-Harness.html
- **TESTS variable:** https://www.gnu.org/software/automake/manual/html_node/Scripts_002dbased-Testsuites.html

### Related Files in geocal-repo
- `lib/test_all.cc` - Test main entry point
- `geocal.am` - Test configuration
- `lib/test_all.sh` - Current test runner wrapper
- `lib/GeoCalCore/Support/unit_test_support.h` - Test utilities

---

## Conclusion

**Recommended starting point:** Implement Approach 1 (Automake Parallel Tests) as a low-risk, quick-win solution that provides 2-4x speedup with minimal changes. If further improvements are needed, Approach 2 can be layered on top.

The key insight is that automake already supports parallel test execution - we just need to organize the tests into independently runnable groups and let `make -j` do its job.
