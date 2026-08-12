# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Git Workflow

**IMPORTANT: Claude should NEVER commit or push git changes directly.**

When making changes to the repository, Claude should:
1. Make the requested code changes
2. Stage files with `git add` 
3. **Stop before committing** - The user will manually review and commit

The user retains full control over commit messages and timing.

## Documentation Practices

### Planning and Analysis Documents

Store planning documents, analysis notes, and technical discussions in the `claude_documents/` directory. This keeps detailed design work separate from code and provides a reference for future sessions.

Examples:
- `claude_documents/parallel_test_execution_plan.md` - Detailed analysis of approaches to parallelize unit test execution
- Future planning documents, architecture analysis, investigation notes

When completing substantial planning or analysis work, save it to a markdown file in `claude_documents/` for future reference.

## Project Overview

GeoCal is a C++ library with Python wrappers for map projection, geometric calibration, and simultaneous bundle adjustment (NPO 20999). The core is a C++ library with Python bindings generated via SWIG. Most user-facing programs are written in Python.

## Build System

GeoCal uses GNU autotools (configure/make). The standard build workflow is:

```bash
# Configure (run from a separate build directory)
../geocal-repo/configure --prefix=/path/to/install

# Build and test
make -j 20 all                # Compile everything
make -j 20 check              # Run C++ unit tests
make install                  # Install (cannot be parallelized)
make -j 20 installcheck       # Run Python tests (requires install)
```

**Important**: Python tests can only run after `make install` because they require the installed Python module.

### Development Build Options

- `--enable-maintainer-mode`: Auto-regenerate configure/Makefile.in when .am files change
- `--enable-debug`: Build with debugging symbols and extra checks (disables optimization)
- `--without-documentation`: Skip doxygen generation during development builds

### Running Single Tests

**C++ tests** (Boost Test framework):
```bash
./lib/test_all --run_test=image_coordinate              # Run a suite
./lib/test_all --run_test=image_coordinate/basic_test   # Run single test
./libtool --mode=execute gdb lib/test_all               # Debug with gdb
```

**Python tests** (pytest):
```bash
cd python
pytest -n 10 tests                          # Run all tests (parallel)
./test.sh tests/tie_point_collect_test.py  # Run single test
```

The `test.sh` script ensures `test_support` is in PYTHONPATH.

## Code Architecture

### Library Structure

```
lib/
├── GeoCalCore/          # Core library classes
│   ├── Support/         # Low-level utilities (Time, coordinates)
│   ├── Interface/       # Abstract base classes (Camera, RasterImage)
│   └── Implementation/  # Concrete implementations (GdalRasterImage)
├── CartLab/             # Legacy cartography lab classes (mostly for backwards testing)
├── Msp/                 # SWIG wrappers for MSP library
├── Mspi/                # MSPI-specific classes
└── Oco3/                # OCO-3 specific classes

python/
├── bin/                 # Python executables
├── geocal/              # Python-only code (not wrapped from C++)
├── tests/               # Python unit tests
└── test_support/        # Test utilities
```

The division between Support/Interface/Implementation is somewhat loose - it's mainly for organizing source files to avoid hundred-file directories.

### SWIG Integration

- C++ classes are exposed to Python via SWIG
- Each C++ header has a corresponding `.i` file (e.g., `camera.h` → `camera.i`)
- SWIG rules are in `swig_rules/` (included as a git subtree)
- Almost all classes derive from `GenericObject` and `Printable<T>` to enable:
  - Automatic casting to correct types in Python
  - Printing via `__str__` (maps to C++ `operator<<`)
  - Serialization support

### Key Dependencies

**Required:**
- Boost ≥ 1.46
- Blitz++ ≥ 0.9
- GSL
- CSPICE (shared library version)
- GDAL
- Python ≥ 3.5

**Optional but commonly used:**
- VICAR RTL & GDAL Plugin
- HDF5, HDFEOS5
- FFTW (for phase correlation)
- Doxygen (documentation)

## File Naming Conventions

- C++ implementation: `foo_bar.cc` / `foo_bar.h`
- C++ tests: `foo_bar_test.cc`
- SWIG interface: `foo_bar.i`
- Python tests: `foo_bar_test.py`
- Autotools fragments: `foo_bar.am`

## Code Style and Tools

### Python

- Use **ruff** for formatting and linting:
  ```bash
  ruff format .      # Format code
  ruff check .       # Run linter
  ```
- Ruff warnings can be silenced in `pyproject.toml` to reduce noise
- **Type hints** are used as documentation, not strict contracts:
  - Add hints where they clarify intent
  - Don't aim for full mypy compliance
  - Duck typing is acceptable even if hints suggest otherwise
  - Current codebase passes mypy cleanly with Python 3.11.10

### C++

- Boost Test framework for unit tests
- Use `operator<<` for printing (automatically wrapped to Python `__str__`)
- Prefer Boost smart pointers (mapped to Python through SWIG)

## Documentation

- **C++ API docs**: Generated by Doxygen during `make install`, output in `<prefix>/share/doc/geocal`
- **Python docs**: Available via `help(class)` in Python (generated from Doxygen)
- **High-level docs**: [GeoCal Documentation](https://github.jpl.nasa.gov/Cartography/GeoCalDocumentation)
- **GitHub Pages**: https://github.jpl.nasa.gov/pages/Cartography/geocal

Update GitHub Pages with:
```bash
make github-pages
```

## Extending GeoCal

To create a new project that extends GeoCal (e.g., adding instrument-specific camera models):
- Use the skeleton: [use-geocal-skeleton](https://github.jpl.nasa.gov/Cartography/use-geocal-skeleton)
- See examples: [ECOSTRESS](https://github.jpl.nasa.gov/ecostress/ecostress-level1), [OCO-3](https://github.jpl.nasa.gov/OCO/oco3-level1)

## Common Gotchas

1. **Install is not parallel**: `make install` must run single-threaded
2. **Python tests need install**: Don't run `pytest` before `make install`
3. **SPICE data**: Set `SPICEDATA` environment variable for SPICE operations (not needed for unit tests)
4. **Libtool and debugging**: Use `./libtool --mode=execute gdb` to debug executables
5. **Autotools regeneration**: Run `./bootstrap` if you modify `configure.ac` or `.am` files
6. **SWIG not needed for builds**: Only needed to regenerate wrappers during development

## Development Workflow

1. Make changes to C++ or SWIG files
2. If adding new classes:
   - Create `.h`, `.cc`, `.i`, and `_test.cc` files
   - Add to appropriate `.am` file
   - Ensure class derives from GenericObject
3. Build: `make -j 20 all && make -j 20 check`
4. Install: `make install`
5. Test Python: `make -j 20 installcheck`

For rapid iteration, configure with `--enable-debug --without-documentation` to skip optimization and doc generation.

## Unit Testing Details

### Boost Test Framework

GeoCal uses Boost.Test (version 1.84.0) for C++ unit tests. Tests are organized into suites using fixtures:

```cpp
#include "unit_test_support.h"
#include "my_class.h"

using namespace GeoCal;
using namespace blitz;

class MyClassFixture : public GlobalFixture {
public:
  MyClassFixture() {
    // Setup test data
    test_orbit.reset(new KeplerOrbit(t0, t1));
  }
  
  Time t0, t1;
  boost::shared_ptr<KeplerOrbit> test_orbit;
};

BOOST_FIXTURE_TEST_SUITE(my_class, MyClassFixture)

BOOST_AUTO_TEST_CASE(basic_functionality)
{
  boost::shared_ptr<MyClass> obj(new MyClass(test_orbit));
  BOOST_CHECK(obj);
  BOOST_CHECK_CLOSE(obj->value(), 42.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(edge_cases)
{
  // Test edge cases
  try {
    MyClass obj(nullptr);
    BOOST_FAIL("Should have thrown exception");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_SUITE_END()
```

### Test Patterns

1. **Fixtures** - Inherit from `GlobalFixture` to set up test data
2. **KeplerOrbit** - Simple analytical orbit commonly used for testing
3. **BOOST_CHECK macros**:
   - `BOOST_CHECK(condition)` - Assert true
   - `BOOST_CHECK_CLOSE(a, b, tolerance_pct)` - Check floating point equality
   - `BOOST_CHECK_MATRIX_CLOSE_TOL(m1, m2, tol)` - Check matrix equality
   - `BOOST_FAIL(msg)` - Explicit failure
4. **Exception testing** - Use try/catch blocks to verify exceptions are thrown
5. **Serialization testing** - Test Boost serialization with `serialize_write_string()` and `serialize_read_string()`

### Current Test Organization

All tests are currently built into a single executable (`geocal_test_all`) that runs all test cases. For approaches to parallelize test execution, see `claude_documents/parallel_test_execution_plan.md`.

### Debugging Common Test Issues

1. **Interval semantics**: Remember that time intervals [a, b) exclude the endpoint b
2. **Floating point comparisons**: Use `BOOST_CHECK_CLOSE` with appropriate tolerance
3. **Exception testing**: Verify the test actually creates the condition that should throw

## Recent Changes and Migrations

### CombineOrbit Migration (2026-06-20)

The `CombineOrbit` class was moved from the `ecostress-level1` repository to `geocal-repo`:

**Changes made:**
- Moved `combine_orbit.{h,cc,i}` from ecostress-level1/lib to geocal-repo/lib/GeoCalCore/Implementation/
- Changed namespace from `Ecostress::` to `GeoCal::`
- Updated serialization macros: `ECOSTRESS_IMPLEMENT` → `GEOCAL_IMPLEMENT`
- Updated SWIG directives: `%ecostress_shared_ptr` → `%geocal_shared_ptr`
- Updated includes: `ecostress_serialize_support.h` → `geocal_serialize_support.h`
- Added comprehensive unit test: `combine_orbit_test.cc` with 9 test cases
- Updated build files: `implementation.am` to include new sources and tests

**Python code changes:**
- Code using CombineOrbit should import from `geocal` instead of `ecostress`:
  ```python
  from geocal import CombineOrbit
  ```

**Serialization compatibility:**
- Old serialized `Ecostress::CombineOrbit` objects are NOT compatible with new `GeoCal::CombineOrbit`
- This is acceptable for development work where re-serialization is feasible
