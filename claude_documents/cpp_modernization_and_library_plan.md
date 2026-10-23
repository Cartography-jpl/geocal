# GeoCal C++ Modernization and Third-Party Library Plan

**Date:** 2026-10-22 (revised the same day after maintainer feedback)
**Status:** Planning only. Nothing has been implemented.
**Scope:** The C++ library (`lib/`), its build system, and the third-party C++
libraries it depends on. Python-only code is covered only where the C++ work
affects it (the SWIG bindings, pickling).

This document builds on `CODE_QUALITY_ANALYSIS.md` and `progress.md` (both
from 2026-05-12) and on the other notes in `claude_documents/`. Those earlier
documents get some facts wrong, so every number below was measured from the
current tree (`git ls-files`, generated `*_wrap.cc` excluded) and from the
reference pixi environment at `../geocal-build/build-pixi`.

## Branch strategy and constraints from the maintainer

**Branches.**
* **`develop_2_0`** is where the modernization work happens (the current
  branch, 9 commits ahead of `master` as of this writing). GeoCal 2.0 is
  allowed to **break the API**: `std::shared_ptr`, a possible move to
  nanobind, C++20.
* **`master` → `maintain_1.28`** (name confirmed; `configure.ac` on
  `master` is version 1.28). Branch it off before 2.0 lands. It serves the
  **closed gcc 11.2 system** and any other user who needs the 1.x API:
  **bug fixes only**, C++17, current dependencies. If a new feature is ever
  needed on 1.x, it is treated as a **backport**: implement it first on
  `develop_2_0`, then port it back to `maintain_1.28`.
* The gcc 11.2 system is a closed, tightly controlled system. It uses
  conda/pixi, but updating packages there takes a very long time. That makes
  a frozen maintenance branch a better fit than holding the whole project at
  gcc 11.2's feature level.
* Since `maintain_1.28` covers gcc 11.2, **2.0 no longer has the gcc 11.2
  constraint.** The rule for 2.0 is: **stable, current conda-forge
  compilers, nothing cutting-edge** (§7).

**Other constraints.**
* **macOS is best-effort.** No sponsor needs it, and it has broken from time
  to time over the years. Keep it working when that is cheap, but it must not
  drive decisions. In particular, libc++ gaps don't block using libstdc++
  features; where two options cost the same, pick the portable one.
* **Boost:** 2.0 can require a current Boost (≥ 1.84, the current
  conda-forge version).
* Replacing GSL is **desirable** if it isn't too expensive.
* Blitz++: we currently use the **conda-forge `libblitz`** package, which is
  sufficient. We maintain our own conda packages in `../afids-conda-package`,
  so we can build our own Blitz there if it is ever needed. GeoCal does not
  carry a copy of Blitz.
* General code cleanup is wanted where it is **reasonably mechanical**. We
  don't want a large manual rewrite.

---

## 1. Corrections to the earlier analysis

| Earlier claim (CODE_QUALITY_ANALYSIS.md) | Actual state |
|---|---|
| "Pre-C++11 codebase" / "Adopt C++14" | `config/m4/afids_common.m4` already has `AX_CXX_COMPILE_STDCXX([17], [ext], [mandatory])`, so the build requires **gnu++17**. The *idioms* are still C++98-era, but the language level is not the problem. |
| "No lambda functions", "no `override`" | There are a few of each (for example, `geocal_thread_pool.h` uses a lambda, and `override` appears 31 times). Neither is used consistently. |
| Replace `boost::function`, `boost::bind`, `boost::unordered_map` | **None of these appear in the code at all.** The Boost features that actually need attention are different (§3.2). |
| "Keep Boost for Blitz++ integration" | Blitz++ does not use Boost. The real tie between Boost and Blitz is the custom serialization of `blitz::Array` (`blitz_array_serialize_support.h`). |
| Boost ≥ 1.46 | `configure.ac` still says 1.46, but the code needs much newer: `boost::asio::thread_pool` needs ≥1.66, and `boost_pcall.h` has Boost.Process v1/v2 logic. The `boost_fix/1.56–1.58` workaround directories are dead code. |
| C++17 is "too new" (comment in `geocal.am`, line ~22, about `BOOST_FILESYSTEM_VERSION=3`) | Out of date. C++17 is already mandatory, so `std::filesystem` can be used. |

Recommendations from the earlier documents that **this plan does not
endorse**, or treats as separate decisions to make later: switching SWIG to
pybind11 as a near-term step, switching autotools to CMake as a prerequisite,
moving to "C++14" (we are already past that), and replacing Blitz++. The
reasons are in §3, §4, §6 and §7.

---

## 2. Current state

### 2.1 Code size (lib/, excluding generated wrappers)

* 655 `.h`/`.cc` files, about 106.5k lines; 230 `.i` SWIG files; 181 `*_test.cc` files.
* About 707 source files in `GeoCalCore` (Implementation 431, Interface 166,
  Support 110). There are smaller trees for CartLab, Mspi, Msp, and Oco3.
* Generated SWIG wrappers are **committed to git** on purpose, so that
  downstream users don't need SWIG (a deliberate trade-off; other repos made
  the opposite choice). They live in `bindings/python/swig/`,
  with 227 `*_wrap.cc` and 226 `.py` files, about 107 MB. Any mass rename
  in the C++ code (for example `boost::shared_ptr` → `std::shared_ptr`)
  regenerates every one of these files, which produces a huge diff.

### 2.2 Toolchains

**Reference development environment** (pixi env `../geocal-build/build-pixi`):

| Component | Version | Notes |
|---|---|---|
| gcc/g++ (conda-forge) | 15.1.0 | |
| Boost | 1.84.0 | conda-forge. |
| Blitz++ | 1.0.2 (conda-forge `libblitz`) | Upstream is in maintenance mode. We can build our own in `../afids-conda-package` if needed. |
| GSL | 2.8 | **GPL-3.0.** |
| GDAL | 3.11.4 | PROJ 9.6.2. |
| CSPICE | N0067 | |
| HDF5 / HDFEOS5 | 1.14.6 / 5.1.16 | |
| FFTW | 3.3.10 | **GPL-2.0+.** Optional. |
| SWIG | 4.3.1 | `env/pixi.toml` still pins `<4.4.0`, but the 4.4 problem has been fixed and **4.4 can be used now**, so the pin can be dropped. |
| Doxygen | 1.13.2 | |
| Python / numpy | 3.13.7 / 2.3.3 | |
| autoconf / automake / libtool | 2.72 / 1.17 / 2.5.4 | |
| fmt, spdlog | 11.2 / 1.15 | Already in the env; GeoCal does not use them. |
| Eigen, pybind11, nanobind, cmake, clang-tidy, clang-format, bear | **not present** | Would need to be added to the pixi env. |

**Other environments:**
* The `env/linux` lock file is on gcc/libstdc++ **15.2**, and `env/osx` is on
  **clang 21 / libc++ 21** (osx-arm64). libc++ is slower than libstdc++ to
  implement some library features, but because macOS is best-effort this
  doesn't constrain 2.0.
* **gcc 11.2 (closed system)** is supported by the `maintain_1.28` branch
  only, not by `develop_2_0`.

**Proposed minimum compiler for 2.0:** **gcc ≥ 14**, in **C++20** mode
(§7.4). clang/libc++ is best-effort.

**Newer compilers are an option.** conda-forge has **gcc 16.2.0** (built
2026-09-16), and the maintainer can move the pixi environment to a newer gcc
if it matters. I tested gcc 16.2 in a throwaway env (§7.3, §7.5). Both have been available in conda-forge for a long
time; development uses whatever the pixi locks give (currently gcc 15 /
clang 21).

### 2.3 Third-party usage (measured)

Count = occurrences / number of files.

**Boost**

| Feature | Uses / files | Where it appears in the API |
|---|---|---|
| `boost::shared_ptr` | 2917 / 427 | **Everywhere:** the public API, SWIG typemaps (`swig_rules/include/shared_ptr_type_mapper.i`, `my_shared_ptr.i`), and a custom serializer (`swig_rules/lib/serialize_shared_ptr.h`, which handles SWIG directors). Downstream repos use it too. |
| `boost::make_shared` / `dynamic_pointer_cast` / `weak_ptr` | 283 / 64 / 6 | Goes with `shared_ptr`. |
| `boost::serialization` | 428 / 216 | Core feature. Python pickling (`%pickle_serialization`, 245 classes) goes through it. Persisted archives are used in production and in `unit_test_data`. |
| `boost::array` | 701 / 105 | Mostly `array<double,3>` and `array<AutoDerivative<double>,3>`. SWIG typemaps in `swig_boost_array.i`. Serialized. |
| `boost::math::quaternion` | 458 / 70 | Core math type, also used with `AutoDerivative`. |
| `BOOST_FOREACH` | 106 / 39 | |
| `boost::filesystem` | 70 / 23 | Pinned to `BOOST_FILESYSTEM_VERSION=3`. |
| `boost::format` / `lexical_cast` | 62 / 16, 55 / 20 | |
| `boost::multi_array` | 51 / 21 | Mostly tests. |
| `boost::regex` | 37 / 12 | Parsing (time strings, CartLab orbit files, SPICE helper, Mspi config). |
| `boost::optional` | 33 / 7 | RSM classes, `tre_support.h`. SWIG typemap `swig_boost_optional.i`. Serialized. |
| `boost::noncopyable`, `scoped_ptr` | 19 / 16, 7 / 3 | |
| `boost::date_time` | 14 / 4 | `geocal_time.cc` (string parsing) and Msp. |
| `boost::iostreams` | 8 / 3 | `ostream_pad.h` and Mspi (gzip). |
| `boost::asio`, `boost::process` | 1–2 files | `geocal_thread_pool.h`, `boost_pcall.h`. |
| `boost::operators` | 4 headers | Header-only and harmless. |
| `boost::math::tools::toms748_solve` | 1 | `geocal_gsl_root.cc`. |
| Boost.Test | all 181 tests | |
| Vendored "boost::backtrace" | `lib/GeoCalCore/Support/boost/backtrace.{hpp,cpp}` | An old, unofficial library that was never accepted into Boost. `geocal_exception.h` inherits from it. |

**Blitz++**: 1843 `blitz::` occurrences in 232 files, and 2045 `Array<` in
267 files. The most common types are `Array<double,2>` (830) and
`Array<double,1>` (612), but ranks up to 7 are used. Blitz is also part of
the zero-copy numpy typemaps (`swig_rules/include/swig_array_inc.i`),
`AutoDerivative<T>` (1126 uses), and serialization.

**GSL**: 15 files outside the `geocal_gsl_*` wrappers. Details are in §3.4.

**FFTW** (3 files): `geocal_fftw.{h,cc}` and `phase_correlation_matcher.cc`. Optional.

**libsdbx** (`thirdparty/libsdbx`, 467 files / about 68k lines, vendored, 2008):
used **only** by `lib/GeoCalCore/Implementation/location_to_file.h` for an
R*-tree. That class is used by `GdalMultiFile` and `CartLabMultifile`. See §3.6.

**HDF5 C++ API** (`H5::`, 2 files), **GDAL** (528 / 60, already updated for
GDAL 3 axis order), **CSPICE** (31 files, kept behind the
`toolkit_*_interface` classes, not thread-safe), VICAR RTL (optional),
GeoTIFF, MSP (optional), MSPI-shared/HDFEOS5 (optional),
`thirdparty/libraw` (disabled).

---

## 3. What should change: library-by-library trade-offs

### 3.1 Recommendation summary

| Library | Recommendation | Risk | Effort |
|---|---|---|---|
| Boost features that have direct `std` equivalents (filesystem, regex, optional, format, lexical_cast, foreach, noncopyable, scoped_ptr, multi_array) | **Replace** | Low | S–M |
| Vendored `boost::backtrace` | **Replace** with Boost.Stacktrace | Low | S |
| `boost::array` → `std::array` | **Replace** (separate step, because it touches SWIG and serialization) | Medium | M |
| `boost::shared_ptr` → `std::shared_ptr` | **Replace**, as a coordinated, versioned break (GeoCal 2.0) | High (downstream impact) | L |
| `boost::date_time` → `std::chrono` | Replace in `geocal_time`; leave the Msp code alone | Low | S |
| Boost.Asio thread pool / Boost.Process / Boost.Iostreams | Keep | — | — |
| `boost::math::quaternion`, Boost.Serialization, Boost.Test | **Keep** | — | — |
| **libsdbx** | **Replace** with `boost::geometry::index::rtree` (header-only, already part of Boost) | Low | S |
| **GSL** | **Replace** with Eigen + Boost.Math | Low–Medium | M (small to medium) |
| FFTW | Replace with pocketfft (§3.5), mainly so `PhaseCorrelationMatcher` can run in threads | Low | S |
| Blitz++ | **Keep** (conda-forge `libblitz`; our own build in `afids-conda-package` as a fallback) | — | — |
| HDF5 C++ API | Optional / low priority: HighFive | Low | S |
| GDAL, PROJ, GeoTIFF, CSPICE, VICAR RTL | Keep; modernize call sites only | — | S |
| SWIG | Keep for now (4.4 is fine); reconsider with the nanobind pilot (§4) | — | — |
| Autotools | Keep for now (§6) | — | — |

### 3.2 Boost: details

These changes are made on `develop_2_0`, with the 2.0 compiler minimums (§7.4).

* `boost::filesystem` → `std::filesystem` (gcc ≥9 needs no `-lstdc++fs`).
  Mechanical. Drop `-DBOOST_FILESYSTEM_VERSION=3`.
* `boost::regex` → `std::regex`. Slower, but every use is parsing small
  headers or config files, so speed doesn't matter. Each regex needs to be
  checked (`std::regex` has no lookbehind, for example).
* `BOOST_FOREACH` → range-for. `noncopyable` → `= delete`. `scoped_ptr` →
  `std::unique_ptr` (§8).
* `boost::format` → **`std::format`** (C++20; gcc ≥13, libc++ ≥17).
  `lexical_cast` → `std::to_string` / `std::from_chars` (or `std::stod`
  where speed doesn't matter, which is also the portable choice for macOS).
* `boost::multi_array` → `blitz::Array` / `std::vector` (mostly in tests).
* `boost::optional` → `std::optional`. Needs a new SWIG typemap, and a
  serializer that writes the same archive layout as `boost::optional`.
* `boost::date_time` → C++20 `std::chrono` calendar types
  (`year_month_day`, `hh_mm_ss`, `sys_time`) for formatting and arithmetic,
  and `std::chrono::parse` (libstdc++ ≥14, which is why the minimum is gcc 14,
  §7.4) for parsing. libc++ has no `parse` yet; because macOS is
  best-effort that is acceptable, but keep the parsing in one function in
  `geocal_time.cc`, so a small fallback parser can be added if macOS ever
  matters.
* **Backtrace**: use Boost.Stacktrace, stored as a member of `Exception`
  rather than as a base class, and add `std::source_location` (C++20) for the
  throw site (§7.1). `std::stacktrace` (C++23) works in gcc 15 but needs the
  separate static `libstdc++exp` library, whose ABI gcc still calls
  experimental. Not something to link into a shared library yet (§7.5).
* **`boost::array` → `std::array`**: the C++ change is mechanical. It needs new
  `swig_rules` typemaps, and the archive layout must be checked with the
  golden archive tests (Phase 0).
* **`boost::shared_ptr` → `std::shared_ptr`**: the largest change. It affects
  the public API, `swig_rules` (shared subtree), the custom
  `serialize_shared_ptr.h` director handling, and `weak_ptr_serialize_support`.
  Do it as GeoCal 2.0, with a migration script for downstream repos and
  wrappers regenerated in a separate commit. Archive compatibility is likely
  (both formats write a `px` NVP) but must be verified.
* **Keep**: Boost.Serialization (no replacement can read existing archives),
  `boost::math::quaternion` (no `std` type; works with `AutoDerivative`),
  Boost.Test, Asio thread pool, Process, Iostreams.
* **`--without-boost-serialization`**: nobody uses it. It is future-proofing,
  so that if GeoCal ever moves to another serialization library, it is clear
  which code to remove. The pattern is cheap: one
  `#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION` per `.cc` file (218 guards in 199
  files, only 5 headers), with `serialize()` only *declared* in headers.
  **But it is currently broken.** I syntax-checked the tree with the macro
  undefined, and **12 files fail**: `argus_orbit.cc`, `vicar_raster_image.cc`,
  `combine_orbit.cc` and its test, `eci_tod_burl.cc`,
  `iterative_morphological_dilation.cc`, `phase_correlation_matcher.cc`,
  `spice_kepler_orbit.cc`, `spice_orbit.cc`, `tle_orbit.cc`, `map_info.cc`,
  `generic_object.cc`. Errors are serialization code outside the guard
  (`GEOCAL_NVP` etc.), plus `geocal_serialize_support.h` and
  `swig_rules/lib/serialize_shared_ptr.h` assuming Boost serialization.
  **Recommendation:** keep the option, because the cost is small. Fix the
  12 files once, and add a **`--without-boost-serialization` build to the
  regular builds** (just compiling the C++ tests is enough). That cheap
  check is what stops it from rotting again. If it keeps breaking in ways
  that are a pain to fix, that is the signal to drop it.

After this, the compiled Boost libraries GeoCal needs drop to
**serialization, unit_test_framework, iostreams (Mspi only)**.

### 3.3 Blitz++

**Current situation:** GeoCal uses the conda-forge `libblitz` 1.0.2 package,
which is sufficient; if a patch is ever needed, we can build our own in
`../afids-conda-package`. Blitz is used everywhere (public API,
`AutoDerivative`, zero-copy numpy typemaps, serialization).

**C++20 compatibility check (done, §7.2):** Blitz headers compile cleanly
in gnu++20 with gcc 15. They produce many *deprecation warnings*
(`-Wdeprecated-copy`, and `-Wvolatile` in `memblock.h`) but **no errors**.

| Option | Pros | Cons |
|---|---|---|
| **Keep Blitz** | No work; behaviour already validated; zero-copy numpy typemaps already work. | Small, ageing upstream. |
| Eigen | Large, active upstream; header-only. | `Matrix` is 2-D only (GeoCal uses ranks 3–7); different slicing; column-major by default; would touch about 270 files plus the typemaps, `AutoDerivative`, and serialization. |
| xtensor | numpy-like API. | Uncertain upstream; heavy compile times; the same amount of rewriting. |
| `mdspan` (via the Kokkos reference implementation, §3.3.1) | Standard design (C++23 `std::mdspan`); available **now** for C++17/20 through the conda-forge `mdspan` package; also provides `submdspan` slicing and an owning `mdarray`. | No expression templates or arithmetic, no `AutoDerivative` integration, so it complements Blitz rather than replacing it. |

**Recommendation:** keep Blitz. Do not add new ways for it to leak into the
API. Where new linear-algebra code is needed (including the GSL
replacement), use Eigen locally through `Eigen::Map` over the Blitz data
(no copy), the same way `GslMatrix` wraps Blitz data today. Treat Blitz
headers as system headers (`-isystem`) so their warnings don't hide GeoCal's
own (§8).

#### 3.3.1 mdspan as an interop layer (reference implementation)

conda-forge's **`mdspan`** package is the Kokkos team's reference
implementation of `std::mdspan` (Apache-2.0 with LLVM exception,
header-only, needs C++14 or later). There are two versions:

| Version | Header / namespace | Contents |
|---|---|---|
| `0.6.0` (the last tagged release, older) | `<experimental/mdspan>`, hard-coded `std::experimental` | mdspan, layouts, submdspan, mdarray |
| `0.6.1.dev20260223` (snapshot, 2026) | `<mdspan/mdspan.hpp>`, configurable `MDSPAN_IMPL_STANDARD_NAMESPACE` (default `Kokkos`) and `MDSPAN_IMPL_PROPOSED_NAMESPACE` (`Experimental`) | mdspan, layouts, `submdspan` (P2630), `mdarray` (P1684, owning), `layout_padded` (P2642), `dims` (P2389) |

**Tested** (in a throwaway env, using the 0.6.1 snapshot together with Blitz 1.0.2):
contiguous row-major views of `blitz::Array<double,3>` storage (writes visible
through Blitz), **`layout_stride` views of non-contiguous Blitz slices built
from `stride()`**, `submdspan`, and an owning `mdarray`. All worked with gcc
15.1 in **C++17** and **C++20** and gcc 16.2 in C++23. In C++23 mode it uses
`m[i, j]`; before that it uses `m(i, j)` (macro `MDSPAN_USE_BRACKET_OPERATOR`).

It doesn't forward to the compiler's `std::mdspan` when that exists; it
always defines its own types. So, if we adopt it:
* Add a tiny GeoCal header (for example `geocal_mdspan.h`) with a namespace
  alias, e.g. `namespace gc_md = Kokkos;`. Later it can point at `std` once
  every supported compiler has `std::mdspan` *and* we don't need the extras
  (`submdspan` is C++26, and `mdarray` isn't standardized).
* Pin the snapshot version in `env/pixi.toml`. Upstream hasn't tagged a release
  since 0.6.0, so the dev snapshot is the version that is actually
  maintained (Kokkos itself is built on it). Either pin the snapshot or vendor
  the headers.

**Where it helps GeoCal:**
* **Binding layer:** nanobind's `nb::ndarray` gives pointer, shape and
  strides, which map directly to `layout_stride` mdspan views, with no copy and
  no Blitz `MemoryBlock` tricks. It is a neutral array type between Python and
  C++, independent of Blitz.
* **New interfaces** (raster tiles, GDAL/VICAR buffers, FFT input/output,
  anything currently passing `T*` plus sizes) can take `mdspan` views instead
  of raw pointers or Blitz-specific types. That covers multi-dimensional
  `std::span` cases (§7.1 item 4).
* **Gradual decoupling from Blitz** without a rewrite: Blitz stays for
  arithmetic and `AutoDerivative` work, and mdspan becomes the type used at
  boundaries. Converting either way is zero-copy (pointer, extents, strides).

**Recommendation:** add `mdspan` (pinned snapshot) together with the nanobind
pilot, and use it first at the binding and IO boundaries. It is not needed
for anything before that.

### 3.4 GSL → Eigen + Boost.Math

GSL is required and GPL-3.0, while GeoCal is BSD-3. The usage turned out to
be **small and easy to map**. There are five areas, about 15 call sites
outside the `geocal_gsl_*` wrapper files (about 600 lines including tests):

| GSL use | Call sites | Replacement | Risk |
|---|---|---|---|
| 10-point Gauss–Legendre (`gsl_integration_glfixed`) | `refraction_msp.{h,cc}` | `boost::math::quadrature::gauss<double,10>` (header-only, Boost ≥1.66). Same nodes and weights, so results match to rounding. | Very low |
| LU solve / inverse (`gsl_linalg_LU_*`, `gsl_invert`) | `lsm_matcher.cc`, `rsm.cc` | Eigen `PartialPivLU` over `Eigen::Map`. Same algorithm. | Very low |
| Linear least squares (`gsl_fit`, built on `gsl_multifit_linear`, which uses SVD) | `quadratic_geometric_model.cc`, `rsm_low_order_polynomial.cc`, `rsm_rational_polynomial.cc`, `geocal_rpc.cc` (9 uses) | Eigen `BDCSVD`/`JacobiSVD` (or column-pivoted QR). Also return χ² and covariance if callers use them. | Low (only differs for nearly singular systems) |
| 1-D root, Brent (`gsl_root_fsolver_brent`) and `root_list` | `galileo_camera.cc`, `igc_rolling_shutter.cc`, `ipi.cc`, `rsm.cc`, `geocal_rpc.cc`, `glas_gfm_camera.cc` | `boost::math::tools::toms748_solve` (already used in the `AutoDerivative` overload in `geocal_gsl_root.cc`) | Low (different iterates, same root within tolerance) |
| N-D root (`gsl_multiroot_fsolver_hybrids`, `fdfsolver_hybridsj`) | `glas_gfm_camera.cc`, `rsm.cc`, `geocal_rpc.cc` | Eigen `unsupported/NonLinearOptimization` `HybridNonLinearSolver`. This is a port of MINPACK `hybrd`/`hybrj`, the same Powell-hybrid algorithm that GSL's `hybrids`/`hybridsj` implement. Alternatively, a ~50-line damped Newton using the Jacobian from `VFunctorWithDerivative`. | Medium-low (review test tolerances) |
| `gsl_check` / `<gsl/gsl_errno.h>` | `geocal_exception.h` (public header), `lsm_matcher.cc` | Remove. | Downstream code that used it gets fixed with the 2.0 migration. |

Things to watch:
* This adds **Eigen** as a new required header-only dependency (conda-forge
  `eigen`). Because this lands on 2.0 only, the closed gcc 11.2 system
  (`maintain_1.28`) doesn't need Eigen.
* `gsl_fit`, `gsl_root` and `gsl_invert` are exposed to Python
  (`geocal_gsl_fit.i`, `geocal_gsl_root.i`). **Decision: remove them from
  the API** (C++ and Python) as part of the 2.0 break, with no compatibility
  wrappers. The new functions get library-neutral names (for example
  `linear_least_squares`, `root`, `invert`). Any downstream code that used
  the old names is fixed as part of the 2.0 downstream migration; the
  downstream projects are maintained by the same person.
* `afids-carto`/VICAR may still link GSL, so removing it from GeoCal may not
  remove it from the AFIDS environment.
* Approach: do one row of the table per PR, top to bottom. For the solver
  rows, run old and new code side by side on the unit-test cases before
  deleting the GSL path.

### 3.5 FFTW vs pocketfft

How GeoCal uses FFTW: 2-D r2c/c2r transforms in `geocal_fftw.cc` (plans made
in constructors), and a 2-D complex transform in `phase_correlation_matcher.cc`.
**A new plan is created on every `match` call** (line ~234) on small square
search windows.

What pocketfft would gain:
* **Always available.** A single vendored header (`pocketfft_hdronly.hpp`,
  BSD-3), so there's no configure probe and no `#ifdef HAVE_FFTW`.
  `PhaseCorrelationMatcher` currently throws if GeoCal was built without FFTW.
* **License:** BSD instead of GPL (FFTW is otherwise sold under a commercial
  license from MIT).
* **Thread safety:** FFTW's planner (`fftw_plan_*`) is *not* thread-safe, and
  the matcher creates a plan per call. That is a hidden problem for anything
  using `ThreadPool` or other threads. pocketfft caches plans internally and
  is safe to call from several threads.
* Proven: numpy and `scipy.fft` both use pocketfft.

What we'd lose: raw speed. FFTW with SIMD and a measured plan can be
noticeably faster on large or power-of-two transforms. For phase-correlation
windows that are tens to a few hundred pixels across, the FFT is not the
bottleneck, and planning on every call probably costs more than the
difference in execution time.

**How it is used today:** `PhaseCorrelationMatcher` is not run from threads
now, but running it in threads would be useful. It is usually not a
bottleneck, but can be in some cases, for example **tie-point collection**.
So thread safety is the deciding argument.

| Option | Thread-safe? | Effort | Notes |
|---|---|---|---|
| Keep FFTW, call `fftw_make_planner_thread_safe()` once at start-up (FFTW ≥ 3.3.5) | Yes: planning is serialized behind a lock; executing a plan was already thread-safe | Tiny | Still plans on every `match` call, so threads would contend on the lock exactly in the case we care about. Still GPL and optional. |
| Keep FFTW, create the plan **once per matcher** (or per thread) and reuse it with `fftw_execute_dft` on new arrays | Yes, with the above | Small | Removes the per-call planning cost too. Still GPL and optional. |
| **pocketfft** (BSD, header-only, vendored) | Yes (thread-safe internal plan cache) | Small (3 files) | Always available, GPL-free; somewhat slower than FFTW on large or power-of-two sizes, which doesn't matter for these window sizes. |

**Recommendation:** go to **pocketfft** when threading the matcher becomes
a goal (for example, when tie-point collection is parallelized). It gives
thread safety, availability and licensing in one small change. If that
happens before the FFT work is scheduled, the two FFTW fixes in the table
are a quick interim measure. Either way, add a unit test that runs the
matcher in parallel with `ThreadPool` and compares against serial results.

### 3.6 libsdbx → Boost.Geometry R-tree

libsdbx (2008, about 68k vendored lines, installs its headers into
`$(includedir)/sdbx`) is used only by `LocationToFile`. That class already
hides the R-tree, and its own doc comment says the code could switch to
Boost's R-tree "if libsdbx ever becomes an issue". The Boost.Geometry R-tree
(`boost/geometry/index/rtree.hpp`, Boost ≥1.54) is header-only, maintained,
and part of a dependency we already require. It is present in the env.

Reasons to do it now:
* It removes 467 vendored files, `thirdparty/libsdbx/sdbx.am`, the header
  install, and the doxygen link.
* In the C++20 check (§7.2), libsdbx was the **only** part of the GeoCal
  tree that produced C++20 deprecation warnings (34 ×
  `-Wdeprecated-enum-enum-conversion` in `TreeItem.h`, plus
  `-Wignored-qualifiers`).
* `location_to_file.h` is an installed public header, so currently
  downstream builds also need the sdbx headers.

Implementation notes. The one subtle part is the **interval semantics**:
* `LocationToFile` uses half-open ranges `[x1, x2)`, and sdbx's `overlaps()`
  uses strict `<`, so tiles that share an edge do *not* overlap.
  Boost.Geometry's `intersects` treats touching boxes as intersecting. With
  integer coordinates, store a **closed box `[x1, x2-1] × [y1, y2-1]`** and
  query `[x, x+n-1]`. That gives exactly the old behaviour.
* Use `bgi::rtree<std::pair<box, std::size_t>, bgi::rstar<16>>`. `find`
  takes the first query hit; `find_region` collects all hits.
* Optional improvement: when files overlap, sdbx returns whichever one its
  tree walk reaches first. Sorting hits by insertion index makes the result
  deterministic ("first added wins"). `GdalMultiFile::...find_region(...,1,1)`
  depends on this choice, so record it in the test.
* `location_to_file_test.cc` already exists. Add an edge-adjacency test before
  making the switch.
* `LocationToFile` is not serialized (its owners rebuild it), so archives are
  not affected.

Effort: small (one header, one test, build-file and doc cleanup).

### 3.7 GDAL, HDF5, CSPICE, other vendored code

* **GDAL 3.11**: keep. Modernize call sites: use `GDALDatasetUniquePtr` (RAII)
  instead of raw handles, and the `std::string` versions of WKT export
  instead of `CPLFree` buffers.
* **HDF5 C++**: only 2 files; HighFive is optional.
* **CSPICE**: keep. Document that it is not thread-safe, next to `ThreadPool`.
* **Remove**: `boost_fix/`, `thirdparty/libraw` (disabled), `unused_code/`,
  after confirmation.

---

## 4. Python bindings: SWIG, pybind11, or nanobind

**Stay on SWIG for now**:
* SWIG 4.4 works now; drop the `<4.4.0` pin in `env/pixi.toml`.
* **Keep committing the generated wrappers.** This is a deliberate
  trade-off so downstream users don't need SWIG. With nanobind the question
  disappears: the bindings are ordinary C++ files compiled by the normal
  build, so there is no generator for downstream users to install and no
  107 MB of generated code in git.
* After the `std::shared_ptr` change, pilot nanobind (which supports only
  `std::shared_ptr`) or pybind11 on a small closed group of classes,
  covering pickling, directors, and numpy arrays.

Reasons not to switch now: 230 `.i` files; 961 `%python_attribute`; 245
pickled classes; 9 directors; RTTI downcasting; Doxygen docstrings; and
downstream repos `%import` GeoCal's `.i` files, so they would all break at
once.

**Update (2.0 branch):** a nanobind move is now being considered as part of
the 2.0 API break. Two points connect it to the rest of this plan:
* nanobind supports only `std::shared_ptr`, so the `std::shared_ptr`
  migration must come first (§5 Phase 5 → Phase 6).
* **SWIG constrains how much C++20 can appear in headers.** I tested SWIG
  4.3.1 (the env version); repeat the test after moving to 4.4, which may
  parse more. It parses `override`, `= default`, `noexcept`,
  concept *definitions*, constrained template parameters
  (`template<std::floating_point T>`), `std::span` parameters, attributes,
  and `auto` return types. It **fails** on defaulted `operator<=>`,
  `requires` clauses, `consteval`/`constinit`, and `using enum`. While SWIG
  is in use, those must stay out of SWIG-parsed headers (or sit behind
  `#ifndef SWIG`). nanobind does not parse headers at all (bindings are
  written in C++), so after a move to nanobind the headers can use any C++20
  feature. That is another argument for nanobind if 2.0 goes that way.
* **C++-only users don't need Python with nanobind either.** Keep today's
  split. Today `libgeocal` has no Python dependency: it only holds abstract
  hooks (`swig_type_mapper_base.h`: `is_python_director_check`,
  `do_swig_python_director_setup`, `to_python` returning `void*`), and the
  `Python.h` code lives in the wrapper library. With nanobind:
  * Bindings, trampoline classes (for Python subclasses) and pickling glue
    are compiled only into the extension module.
  * `libgeocal` keeps the abstract hooks; no nanobind types in its headers
    (`mdspan` is the neutral array type at the boundary, §3.3.1). The build
    enforces this, because nanobind headers need `Python.h`.
  * Linux extension modules don't link `libpython` at all; Python's symbols
    are resolved from the interpreter at import time.
  * The costs are the build (nanobind officially supports CMake /
    scikit-build-core; autotools would need nanobind's runtime compiled by
    hand) and pinning the same nanobind version across repos so downstream
    extension modules can share GeoCal types.
  * Packaging can get cleaner: a Python-free `libgeocal` package and a
    separate Python package, possibly one stable-ABI binary for Python ≥ 3.12.

---

## 5. Phased plan

All phases are on **`develop_2_0`**. Each phase leaves the tree buildable
with all C++ and Python tests passing. Phases 0–4 keep the public API almost
unchanged, apart from the C++20 requirement; Phases 5–6 are the API break.

### Phase 0: Baseline and safety net
1. **Create the maintenance branch:** `master` → `maintain_1.28`. It stays
   C++17, keeps the current dependencies, and gets bug fixes only; any
   feature it needs is implemented on 2.0 first and backported. Document in
   the README which branch serves which systems.
2. Do a clean build and run all tests in the pixi env; record the baseline.
3. **Add a minimum-compiler build:** a pixi environment pinned to the 2.0
   minimum (`gxx_linux-64 14.*`, §7.4), run regularly so features newer than
   the minimum are caught when they are added. An occasional osx build is
   optional (macOS is best-effort).
4. Add `clang-tools`, `bear` and `eigen` to `env/pixi.toml`; drop the SWIG
   `<4.4.0` pin.
5. **Golden serialization tests**: existing `unit_test_data` archives and
   pickles of the main classes. Real downstream archives come mainly from
   **ECOSTRESS, where they are embedded in HDF5 files**. Extracting those
   is deferred until the serialization-related phases (4 and 5), and must
   be done before them.
6. Add a **`--without-boost-serialization` build** (compile the C++ tests
   only) to the regular builds, after fixing the 12 files that break it today
   (§3.2).
7. Optional: automake parallel tests (`parallel_test_execution_plan.md`).
8. Add `.git-blame-ignore-revs` for the mass-edit commits.

### Phase 1: C++20 switch, cleanup and hygiene
1. **Require C++20** (§7.4). The change is a single line, but `config/m4`
   is a subtree shared with the other AFIDS repos (`autoconf-repo`), so
   either put the requirement in GeoCal's own `configure.ac` or coordinate
   the change to `afids_common.m4` with those repos. Then do a full build,
   `make check`, `make install` and `make installcheck`, including the SWIG
   wrappers compiled as C++20. Do this first, so later phases can use
   `std::format` etc. directly instead of temporary solutions.
2. Raise the `AC_BOOST` minimum to what the code really needs. Delete
   `boost_fix/`, the disabled libraw, and `unused_code/`.
3. Show third-party headers with `-isystem` (Blitz, Boost, GDAL) so GeoCal's
   own warnings are visible (§8.1).
4. Fix the real issues the compile check turned up (§8.2).
5. Mechanical idiom fixes with clang-tidy (§8.3), one directory per commit,
   with wrapper regeneration in separate commits.
6. Boost.Stacktrace and `std::source_location` replace the vendored
   backtrace (§7.2).

### Phase 2: Small library swaps that are not in the API
1. **libsdbx → Boost.Geometry R-tree** (§3.6).
2. `filesystem`, `regex`, `format` (→ `std::format`), `lexical_cast`,
   `multi_array`, `date_time` (in `geocal_time`, → chrono calendar types) → `std`.
3. Remove the corresponding compiled Boost libraries from the build.

### Phase 3: GSL removal (§3.4)
Gauss–Legendre → LU → least squares → 1-D roots → N-D roots, one PR each,
with old and new results compared side by side. Then remove GSL from
`configure.ac`, `geocal.am` and `geocal_exception.h`. Remove `gsl_check`,
`gsl_fit`, `gsl_root` and `gsl_invert` from the C++ and Python API (no
compatibility wrappers), and fix any downstream callers as part of the 2.0
migration.

### Phase 4: Types that cross into serialization and SWIG
**Before starting:** collect the ECOSTRESS archives embedded in HDF5 files
for the golden tests (Phase 0 step 5).
`boost::optional` → `std::optional`, then `boost::array` → `std::array`,
each checked against the golden archives. Contribute the `swig_rules`
changes back upstream.

### Phase 5: `std::shared_ptr` (GeoCal 2.0 API break)
As described in §3.2, coordinated with `swig_rules` and downstream repos.

### Phase 6: Binding tool (if nanobind is chosen)
Run the nanobind pilot (§4), then decide. Include the pinned `mdspan`
reference implementation as the array type between `nb::ndarray` and Blitz
(§3.3.1). If the move goes ahead, the remaining SWIG limits on headers go
away (§4, §7.2), C++23 becomes worth switching on (§7.5), and CMake becomes
worth reconsidering (§6).

### Phase 7: Optional / later
* FFTW → pocketfft (§3.5). Move it earlier when threading
  `PhaseCorrelationMatcher` (for example in tie-point collection) becomes a goal.
* GDAL call-site RAII; HighFive.
* Reassess C++23 (§7.5) once libc++ has caught up.

---

## 6. Build system: autotools vs CMake

Unchanged: **not a prerequisite** for anything above. clang-tidy can get its
`compile_commands.json` from the autotools build with `bear -- make`.
Revisit CMake if the binding tool changes, or if the sibling AFIDS repos
migrate together.

---

## 7. C++20 for GeoCal 2.0

**Decision (revised):** the gcc 11.2 system is now served by the
`maintain_1.28` branch, so **`develop_2_0` should require C++20**, with the
stable conda-forge compiler minimums in §7.4. The first version of this plan
recommended staying on C++17. That was only because of the gcc 11.2 floor,
whose libstdc++ 11 lacks the most useful C++20 library features
(`std::format`, chrono calendar/parse). Without that floor, the switch costs
almost nothing (§7.3) and the benefits below are real.

### 7.1 What C++20 gives GeoCal, most useful first

Counts are from the current tree.

1. **`std::format`** (libstdc++ 13, libc++ 17). The biggest everyday win.
   * Replaces `boost::format` (62 uses / 16 files) and most of the 20
     `ostringstream` build-a-string helpers.
   * Error messages: there are **163** `Exception e; e << ... ; throw e;`
     blocks (53 files). Add a constructor taking a format string and
     arguments, so these become one line, for example
     `throw Exception("Line {} out of range [0, {})", ln, nline);`. Keep
     `operator<<` working for existing code.
   * Add one generic `std::formatter` for every `Printable<T>` (it calls the
     existing `print()`), so GeoCal objects can be formatted with `{}`.
   * Because the type checks happen at compile time, mismatched arguments
     become compiler errors instead of runtime exceptions from `boost::format`.
2. **`std::source_location`** (libstdc++ 11, libc++ 16). `Exception` can
   record file, line and function of the throw site automatically, through a
   defaulted constructor argument (when combined with variadic format
   arguments this needs a small helper struct, the usual trick). Together
   with Boost.Stacktrace this makes production error reports much more
   useful, without changing any of the existing throw sites.
3. **Concepts** (language). GeoCal has a lot of code that is generic over the
   scalar type `T` = `double` or `AutoDerivative<double>`: `TimeBase<T>`,
   the `quat_rot*<T>` helpers, coordinate conversions, the
   `*WithDerivative` paths. A `GeoCalScalar` concept documents that intent,
   gives readable error messages instead of pages of Blitz/Boost template
   errors, and lets overloads be selected cleanly instead of with
   `enable_if`. Also useful for `ThreadPool::add_task`. Moderate benefit;
   mostly for new code and when code is touched anyway. **SWIG note:** use the
   `template<GeoCalScalar T>` form, which SWIG parses; avoid `requires`
   clauses in SWIG-parsed headers (§4).
4. **`std::span`** (both libraries). There are 178 raw `.data()` uses, many
   paired with a separate length, for example buffers for GDAL `RasterIO`,
   VICAR RTL, FFTW, and raster tile copies. `std::span<double>` as the
   parameter type keeps pointer and length together, gives bounds checks in
   debug builds (`_GLIBCXX_ASSERTIONS`), and can be built from `std::vector`,
   `std::array`, or Blitz `data()`/`size()`. Good for internal interfaces;
   the public array type stays Blitz.
5. **`std::chrono` calendar types** (`year_month_day`, `hh_mm_ss`,
   `sys_days`). These replace the `boost::date_time` formatting and
   calendar arithmetic in `geocal_time.cc`, and `std::chrono::parse`
   (libstdc++ ≥14) replaces its parsing, which removes a compiled Boost
   library. Leave GeoCal's own leap-second / PGS / GPS time handling alone.
   It is validated, tied to SPICE / toolkit data, and the C++20
   `utc_clock`/`gps_clock` would bring a second source of leap-second data.
6. **Threading primitives**: `std::jthread` with `stop_token` (cancellation),
   `std::latch`, `std::barrier`, `std::counting_semaphore`. These would make a
   small `std`-only replacement for the `boost::asio::thread_pool` in
   `geocal_thread_pool.h` simple. Optional; Asio is fine to keep.
7. **Defaulted comparisons / `operator<=>`**: `bool operator==(const X&) const
   = default;` for simple value types (coordinates, `FrameCoordinate`,
   `ImageCoordinate`, …), and `<=>` could replace `boost::operators` in
   `Time`. Small (only about 14 hand-written `operator==`/`operator<` today).
   **SWIG cannot parse defaulted `<=>`**, so this has to wait for nanobind or
   use `#ifndef SWIG`.
8. **Small conveniences:** `std::numbers::pi` replaces the non-standard
   `M_PI` macro (9 uses); `starts_with`/`ends_with` replace
   `substr(0,…)`/`find(...)==0` in parsers (about 11); `contains()` on maps
   and sets; designated initializers for option structs; `std::ssize`
   (fewer signed/unsigned casts in loops); `[[likely]]`; template lambdas;
   `constexpr` `std::vector`/`std::string`.
9. **`<ranges>`**: `std::ranges::sort(v)` and similar algorithms, projections,
   `views::iota`. Useful but small, because most GeoCal loops are index
   loops over Blitz arrays. The C++23 additions (`views::enumerate`,
   `views::zip`) would help those loops more (§7.5).
10. **Not useful for GeoCal now:** modules (build tooling, SWIG and nanobind
    are not ready) and coroutines (no matching use case).

### 7.2 What it costs

* **Code changes needed to compile as C++20: essentially none** (measured,
  §7.3).
* **Downstream projects** that include GeoCal headers must also compile as
  C++20 once those headers use C++20 features. That fits in the 2.0 API
  break anyway.
* **While SWIG is in use**, some C++20 syntax must stay out of SWIG-parsed
  headers (`<=>`, `requires`, `consteval`/`constinit`, `using enum`; tested
  with SWIG 4.3.1, §4). Implementation (`.cc`) files have no such limit.
* **libc++ (macOS) is behind libstdc++** for some library features (§7.4).
  Because macOS is best-effort, that only means some features will need a
  fallback if macOS support is wanted at some point.
* The **gcc 11.2 system cannot build 2.0**. That is handled by `maintain_1.28`.

### 7.3 Is the code ready for C++20? (measured)

I syntax-checked (`-fsyntax-only -Wall -Wextra -Wdeprecated`) all 366
`GeoCalCore`, CartLab and Oco3 `.cc` files with the pixi gcc 15.1, Boost
1.84 and Blitz 1.0.2, once as `-std=gnu++17` and once as `-std=gnu++20`:

* **The same 364 files compile in both modes.** The 2 that don't
  (`gdal_multi_file*.cc`) fail in both because of a missing include path in
  my test harness, not because of the language level.
* **The warning output is identical** in both modes, because
  `-Wdeprecated` already turns on the C++20 deprecation warnings in C++17
  mode. There were no errors from C++20 rewritten comparison operators, the
  usual problem for code using `boost::operators` or expression templates.
* Almost all of the 118,947 warnings come from **third-party headers**
  (Blitz: about 10.8k `-Wdeprecated-copy` and 2.7k `-Wvolatile`; Boost.Asio:
  248 `-Wdeprecated`). The C++20-related warnings inside the GeoCal tree come
  only from **libsdbx**. GeoCal's own code has only 10 `-Wdeprecated-copy`
  warnings plus 1501 `-Wunused-parameter`.
* **C++23 too:** the same harness with `-std=gnu++23` gives the same
  result: the same 364 files compile, and the warning output is identical
  to gnu++20.
* **gcc 16.2, C++23:** same 364 files compile. gcc 16 adds 61
  `-Wunused-but-set-variable` warnings, 60 in Boost headers
  (`concept_check.hpp`, process `utf8.ipp`) and 1 in GeoCal
  (`pan_sharpen_test.cc:96`).
* Not covered: Msp and Mspi (their optional dependencies weren't set up),
  the SWIG wrapper `.cc` files, linking, running the tests, and clang/libc++.
  Phase 1 step 1 (full build and all tests as C++20) closes those gaps.

### 7.4 Proposed minimum compiler for 2.0

"Stable, readily available in conda-forge, nothing cutting-edge", with
macOS best-effort:

| | Minimum | Why this one | Development (current pixi locks) |
|---|---|---|---|
| Linux, gcc / libstdc++ | **gcc 14** | Completes the C++20 *library* (chrono `parse` and time zones, on top of `std::format` from gcc 13); also has nearly all C++23 language features if we move to C++23 later (§7.5). Released 2024, long available in conda-forge. | gcc 15.2 |
| macOS, clang / libc++ | best-effort | libc++ 21 (current osx lock) has `std::format`, `jthread`, `mdspan`, but no `chrono::parse` and no `std::stacktrace`. | clang 21 / libc++ 21 |
| Standard | **C++20** (`AX_CXX_COMPILE_STDCXX([20], [noext], [mandatory])`) | `noext` (strict ISO) if nothing depends on GNU extensions; otherwise keep `ext` | |

Where a libstdc++-only feature is used (for example `chrono::parse`), keep
it in one place, so a fallback can be added if macOS support is ever
wanted.

### 7.5 C++23: worth considering? (measured, gcc 15.1)

**State of gcc.** The C++23 *language* is essentially complete in gcc 14/15.
The *library* is mostly there in gcc 15 and gcc 16 adds `<mdspan>`. I
tested with the pixi gcc 15.1 (`-std=c++23`, `__cplusplus == 202302`), and
with gcc 16.2 (conda-forge) for `mdspan`:

| C++23 feature | gcc 15.1 | First gcc | Useful to GeoCal? |
|---|---|---|---|
| `std::expected` | ✅ | 12 | **Moderate.** A non-throwing return type for code that fails often, for example `ImageGroundConnection` calls that throw `ImageGroundConnectionFailed`, or solvers throwing `ConvergenceFailure` inside loops. Callers can test the result instead of catching exceptions. Keep exceptions in the public API that Python sees. |
| `views::enumerate`, `views::zip`, `cartesian_product`, `ranges::to` | ✅ | 13 / 14 | **Moderate.** These are the views that actually help with GeoCal-style loops: `for (auto [i, x] : views::enumerate(v))`, zip over parallel arrays, and nested line/sample loops. Check performance before using them in hot raster loops. |
| `std::print` / `println` | ✅ | 14 | Small: GeoCal mostly prints through `operator<<` and `print()`; useful in tools and tests. |
| `std::flat_map` / `flat_set` | ✅ | 15 | Small to moderate: sorted-vector maps for read-mostly lookup tables, such as time-keyed orbit and time-table maps used with `lower_bound`. Faster to search than `std::map`. gcc 15 only. |
| `std::stacktrace` | ✅ with `-lstdc++exp` | 12 (exp) | Would replace Boost.Stacktrace, but `libstdc++exp` is a separate static library that gcc still calls experimental. Not for a shared library yet. |
| Deducing `this` | ✅ | 14 | Could simplify the `Printable<T>` CRTP base and const/non-const accessor pairs. **SWIG 4.3.1 cannot parse it** (tested), so only after nanobind. |
| Multidimensional `operator[]` (`a[i, j]`) | ✅ | 12 | Little: Blitz uses `operator()`. SWIG parses it (tested). |
| `std::mdspan` | ❌ missing in gcc 15.1; ✅ **in gcc 16.2** (tested, rank-3 and `layout_left`) | 16 | The most interesting one for GeoCal, but **it doesn't need C++23 or gcc 16**: the conda-forge `mdspan` reference implementation gives the same thing (plus `submdspan`/`mdarray`) in C++17/20 (§3.3.1). |
| `if consteval`, `[[assume]]`, `static operator()`, `std::unreachable`, `std::to_underlying`, `string::contains`, `std::move_only_function`, `std::out_ptr` | ✅ | 11–14 | Small conveniences. `out_ptr` is handy for C APIs (GDAL, SPICE) that hand back pointers via out-parameters; `move_only_function` suits thread-pool task queues. SWIG parses `if consteval`, `[[assume]]`, `static operator()` (tested). |
| `std::generator` | ✅ | 14 | Little: lazy tile iteration could use it, but nothing needs it. |

**Cost.** Moving from C++20 to C++23 costs no code changes (measured with gcc
15.1 and 16.2, §7.3). The real costs are:
1. The minimum compiler effectively becomes gcc 14 (gcc 15 for `flat_map`,
   gcc 16 for `std::mdspan`). Moving the pixi env to a newer gcc is possible,
   so this isn't a blocker.
2. **ABI caution:** GCC's documentation still describes its C++23 support as
   experimental, meaning the layout of new library types may still change
   between gcc releases. That matters for a shared library: keep C++23
   library types (`std::expected`, `flat_map`, …) **out of the public API**
   until gcc drops the experimental label, or downstream code built with a
   different gcc could break.
3. SWIG can't parse deducing `this` (in addition to the C++20 items in §4).

**Recommendation:** C++23 is **not compelling enough to make it the standard
today**. Most of the real benefit is in C++20, which delivers `format`,
`source_location`, concepts, `span`, and chrono. C++23 adds conveniences
(`expected`, enumerate/zip, `print`, `flat_map`). The most useful new piece
for GeoCal, `mdspan`, can be had in C++20 from the reference implementation,
so it's no longer a reason to move. The ABI is not declared stable either.
However, it would also cost almost nothing. Two reasonable choices:

* **(A, preferred) Require C++20 with gcc ≥ 14 now; move to C++23 later.**
  The switch is one line, because the code already compiles as C++23. A
  natural time is **when SWIG is gone** (nanobind, Phase 6): then deducing
  `this` and the other syntax SWIG can't parse become usable in headers, and
  that is when C++23 pays off most.
* **(B) Require C++23 with gcc ≥ 14 now**, with a written rule: C++23
  library types only in implementation files, not in public headers, until
  the ABI is stable. That gives `expected` and enumerate/zip internally right
  away. It's not wrong, but the gain over (A) is small, and it adds a rule
  everyone has to remember.

---

## 8. General mechanical code cleanup

The goal is changes that are **mostly done by tools, easy to review, and
don't change the public API**. They are done on `develop_2_0` in C++20 mode.
Tooling: clang-tidy, with `compile_commands.json` from `bear -- make`, run
per directory with `-fix`, followed by a build and the full test suite.

### 8.1 Make warnings useful first
Today, 118,947 warnings bury the roughly 1,560 that come from GeoCal's own
code. Add third-party include directories with `-isystem` instead of `-I`
(in the `ac_blitz.m4`, `ac_boost.m4`, `ac_gdal.m4`… flags, or with a
GeoCal-side rewrite of the flags). Then decide about `-Wunused-parameter`
(1501): either turn it off, or fix it only in code that is touched anyway.
Unnamed parameters in virtual overrides are normal.

### 8.2 Real issues the compile check found (fix by hand, tiny)
* `lib/GeoCalCore/Support/dir_change.h`: the **destructor throws**
  (`-Wterminate`, which means `std::terminate` will be called). The
  constructor also leaks `dirhandle` if `chdir` fails. Log or ignore the
  error in the destructor, and close the handle on the constructor's error
  path.
* `MapInfo` has a user-declared copy constructor but an implicit copy
  assignment (`-Wdeprecated-copy`, seen through `srtm_dem.h` and
  `dem_map_info_offset.h`). Add `operator=` (or `= default` both).

### 8.3 Mechanical modernization (clang-tidy checks, with measured scope)

| Change | Scope | Tool / check | Notes |
|---|---|---|---|
| Add `override` to overriding virtuals | Hundreds (387 `virtual …;` declarations, only 31 `override` today) | `modernize-use-override` | Biggest benefit: the compiler catches signature mismatches. SWIG handles `override`. |
| `throw()` → `noexcept` | 15 | `modernize-use-noexcept` | |
| `NULL` → `nullptr` | 63 | `modernize-use-nullptr` | |
| Empty `~X() {}` / `X() {}` → `= default` | 307 / 185 | `modernize-use-equals-default` | SWIG handles `= default`. |
| `boost::noncopyable` → `= delete` | 19 | `modernize-use-equals-delete` + manual | |
| `boost::shared_ptr<X> p(new X(…))` / `p.reset(new X(…))` → `auto p = boost::make_shared<X>(…)` | 261 / 162 | `modernize-make-shared` with `MakeSmartPtrFunction: boost::make_shared` | One allocation instead of two. Fails for private constructors; clang-tidy skips those. Carries over unchanged to `std::` in Phase 5. |
| `scoped_ptr` → `std::unique_ptr` | 7 | manual | |
| Container loops → range-for | ~60 (`.size()` index loops and explicit-iterator loops) + 106 `BOOST_FOREACH` | `modernize-loop-convert`; `BOOST_FOREACH(decl, range)` → `for(decl : range)` with a simple script (macro arguments can't contain unprotected commas, so a regex is safe) | **Leave the ~1000 numeric index loops** (`for(int i = 0; i < number_line; ++i)`) alone: they index Blitz arrays or pixel positions, and range-for doesn't apply. |
| `auto` for iterators, `new`, and casts | 25 iterator declarations + the make_shared sites above | `modernize-use-auto` (default settings only cover iterators/new/casts) | See the Blitz warning below. |
| `<math.h>` etc. → `<cmath>` | 11 | `modernize-deprecated-headers` | |
| `typedef` → `using` | 134 | `modernize-use-using` | Optional: readability only. SWIG 4 handles aliases. |
| `M_PI` → `std::numbers::pi` (C++20) | 9 | manual / sed | Removes a non-standard macro. |
| `substr(0,n) == "..."` / `find("...") == 0` → `starts_with` (C++20) | ~11 | `modernize-use-starts-ends-with` (clang-tidy ≥18) | |
| `Exception e; e << …; throw e;` → format-string constructor (C++20) | 163 / 53 files | **Not mechanical**: convert opportunistically when touching code | Needs the new `Exception` constructor (§7.1 item 1) first. |
| C-style numeric casts → `static_cast` | 413 | — | **Skip**: too noisy for the benefit. |

### 8.4 Things *not* to do
* **No blanket `auto` on Blitz expressions.** `auto x = a + b;`
  (where `a` and `b` are `blitz::Array`) holds an *expression template* that
  refers to its operands, not an `Array`. It evaluates lazily and can dangle.
  The same goes for `auto` on results of `blitz::Range` slices that are
  expected to copy. Restrict `auto` to iterators, `make_shared`, and casts.
* **No `auto` return types or trailing return types in public headers.**
  SWIG parses those headers and can't deduce return types.
* No signature-changing checks (`modernize-pass-by-value`,
  `use-trailing-return-type`, `use-nodiscard`). They change what SWIG and
  downstream code see, causing churn for little gain.
* No codebase-wide reformat unless the maintainers choose to do it (§5
  Phase 1 / `.clang-format`).
* Each check is its own commit, and so is each wrapper regeneration. Add the
  commits to `.git-blame-ignore-revs`.

---

## 9. Risks and how to handle them

| Risk | Mitigation |
|---|---|
| Accidentally using features newer than the minimum compiler | Phase 0 minimum-compiler (gcc 14) build, run regularly. |
| Fixes needed on both `maintain_1.28` and `develop_2_0` | Keep 1.x to bug fixes only; fix on `maintain_1.28` first and merge or cherry-pick forward while the code bases are still close. Features go the other way: 2.0 first, then backport. |
| `--without-boost-serialization` rots again | A regular C++-only build with the option (Phase 0 step 6); drop the option if it becomes a burden. |
| Threaded `PhaseCorrelationMatcher` hitting FFTW's non-thread-safe planner | pocketfft, or the FFTW interim fixes in §3.5, before parallelizing tie-point collection; parallel-vs-serial unit test. |
| C++20 syntax in headers breaks SWIG | Keep `<=>`, `requires`, `consteval`/`constinit`, `using enum` out of SWIG-parsed headers until nanobind (§4). |
| Old archives or pickles can no longer be read | Golden archive tests; keep NVP names and layout. |
| Downstream breakage from `std::shared_ptr` | GeoCal 2.0, migration script, convert reference downstream repos first. |
| Numeric differences after replacing GSL solvers | One solver family at a time; side-by-side comparison; explicit tolerance review. |
| Changed tile selection when files overlap after the R-tree swap | Edge-adjacency and overlap tests before the switch; deterministic ordering. |
| clang-tidy edits breaking SWIG parsing | Only SWIG-compatible checks (§8.4); regenerate wrappers and run the Python tests after each check. |
| Huge diffs from regenerated wrappers | Separate commits for wrapper regeneration (the wrappers stay in git by design until a nanobind move removes them). |

---

## 10. Decisions and open questions

**Decided (maintainer answers):**
* Maintenance branch `maintain_1.28`: bug fixes only; any feature needed
  there is implemented on 2.0 first and backported.
* macOS: best-effort. Boost: 2.0 requires ≥ 1.84. Newer gcc (up to the
  current conda-forge 16.2) is acceptable if it matters.
* `gsl_check`, `gsl_fit`, `gsl_root`, `gsl_invert`: removed from the API in
  2.0; downstream is fixed as part of the migration.
* SWIG 4.4: the old problem is fixed; 4.4 can be used.
* Generated SWIG wrappers stay committed (deliberate trade-off so downstream
  doesn't need SWIG); nanobind would remove the issue.
* `--without-boost-serialization`: keep while it's cheap; it's currently
  broken in 12 files (§3.2). Fix, and add a regular build to keep it working.
* `PhaseCorrelationMatcher` is not threaded today, but threading it (for
  example in tie-point collection) is desirable, so FFT thread safety matters (§3.5).

**Still open:**
1. **Downstream archive inventory.** ECOSTRESS is probably the only
   producer of persisted archives, and they are embedded in HDF5 files.
   Revisit when the serialization work starts (before Phases 4–5).
2. **When to cut `maintain_1.28`.**
