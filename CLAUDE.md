# CLAUDE.md

Header-only C++17 library of error-correcting codes and related building blocks
(CRC, PRNGs, Galois/prime fields, Reed–Solomon, BCH, LDPC, polar, PAC, OSD,
erasure coding, SIMD wrappers). See README.md for an overview of each header.

## Layout

- `*.hh` in the repository root: the library itself, all header-only.
- `tests/`: one `*_test.cc` per test, built and run by `tests/Makefile`.
- `tools/`: small helper programs.

## Building and testing

```
cd tests && make
```

`make` builds every `*_test.cc` and runs it, with stdout discarded. The runs are
joined with `;`, so a failing test does not necessarily make `make` fail.
Look for assertion messages on stderr.
The default compiler is `clang++ -stdlib=libc++ -march=native`. The Makefile has
commented-out lines for `g++` and for ARM cross-compilation under QEMU.
Run `make clean` to remove the binaries.

Tests are compiled with `-std=c++17 -O2 -fno-exceptions -fno-rtti -ffast-math`,
so code must:

- not use exceptions or RTTI;
- stay correct under `-ffast-math`. Don't rely on signed zeros, NaN or infinity
  checks, or a strict order of floating-point operations. For example, the polar
  constructions store erasure probabilities above one half as their negated
  complement and clamp values to `std::numeric_limits<double>::min()`, so that
  signs survive underflow.

## Code style

- Every header starts with a short C comment block: a one-line description,
  a blank line, then `Copyright <year> Ahmet Inan <inan@aicodix.de>`.
- Next comes `#pragma once`, then the includes, then everything inside `namespace CODE {`.
- Indent with tabs and use K&R braces. Keep the code compact and avoid
  unnecessary abstraction.
- Most classes are templates sized at compile time (for example `template <int MAX_M>`)
  with fixed-size member arrays. Avoid heap allocation.
- The main entry point is usually `operator()`.
- Keep the README section for a header up to date when its public behaviour changes.

## Conventions worth knowing

- Polar reliability orderings (`BhattacharyyaSequence`, `ReedMullerSequence`,
  `BhattacharyyaFreezer`) sort from least reliable to most reliable index.
  Ties are broken by the Hamming weight of the index and then by the index
  value, the same way as `ReedMullerSequence`.
  Keep new constructions consistent with this.

## Workflow

- Commit messages are a single short lowercase line, such as
  "break ties in the same order as ReedMullerSequence". There is no body and
  no trailers: no `Co-Authored-By`, no session links.
- Don't push. Deliver changes as a `git format-patch` patch for the
  maintainer to review and apply.
- When changing an algorithm, check that its output is unchanged or as
  intended, for example by comparing old and new versions over many
  parameters, and say how you checked it.
