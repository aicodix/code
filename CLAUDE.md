# CLAUDE.md

Header-only C++17 library of error-correcting codes and related building blocks
(CRC, PRNGs, Galois/prime fields, Reed–Solomon, BCH, LDPC, polar, PAC, OSD,
erasure coding, SIMD wrappers). See README.md for an overview of each header.

## Layout

- `*.hh` in the repository root: the library itself, all header-only.
- `tests/`: one `*_test.cc` per test, built and run by `tests/Makefile`.
- `wasm_tests/`: tests of the WebAssembly SIMD128 backend, built without the
  standard library and run with Node.js.
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

`cd wasm_tests && make` builds `simd_test.cc` twice with clang for `wasm32`,
with `-msimd128` for the `simd128*.hh` backend and without it for the
generic code in `simd.hh`, and compares their results with `simd_test.mjs`
in Node.js. It needs no WASI toolchain: `wasm_tests/quirks/` stands in for
the few standard headers `simd.hh` includes.

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

## Tools

- `tools/rank.cc` ranks the full period triples of `XorShiftMask` with 8 to
  24 bits, as interleavers, by how evenly neighbouring indices spread.
- `tools/burst.cc` ranks the full period triples of `XorShiftMask` with 3 to
  10 bits, as permutations of the N = 1 << BITS bits of a codeword of
  `HadamardEncoder<BITS+1>` on adjacent tones, index 0 fixed, by the least
  Hamming distance left between the codewords outside a burst of erased
  tones. The best triples below all reach the limit of N - BITS - 1 for the
  longest burst, about twice the N / 2 - 1 without permutation, and the
  triples listed for each BITS tie on all values. Mirrored triples, a b c
  and c b a, tied in every case listed here:

  | BITS | longest burst | best triples (a b c) |
  |---|---|---|
  | 3 | 4 | 1 1 1, 1 2 2, 2 2 1 |
  | 4 | 11 | 1 3 2, 1 3 3, 2 1 3, 2 3 1, 3 1 2, 3 3 1 |
  | 5 | 26 | 1 1 2, 1 4 2, 1 4 3, 1 4 4, 2 1 1, 2 4 1, 3 2 4, 3 4 1, 4 2 3, 4 4 1 |
  | 6 | 57 | 1 3 3, 3 3 1 |
  | 7 | 120 | 3 2 5, 5 2 3, 5 2 6, 6 2 5 |
  | 8 | 247 | 1 7 3, 1 7 6, 1 7 7, 3 7 1, 6 7 1, 7 7 1 |
  | 9 | 502 | 6 2 7, 7 2 6 |
  | 10 | 1013 | 1 1 3, 3 1 1 |

  The modem uses 6 1 3 3 for the 64 seed tones.

## Workflow

- Commit messages are a single short lowercase line, such as
  "break ties in the same order as ReedMullerSequence". There is no body and
  no trailers: no `Co-Authored-By`, no session links.
- Don't push. Deliver changes as a `git format-patch` patch for the
  maintainer to review and apply.
- When changing an algorithm, check that its output is unchanged or as
  intended, for example by comparing old and new versions over many
  parameters, and say how you checked it.
