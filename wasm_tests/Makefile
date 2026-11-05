
CXX = clang++
CXXFLAGS = -I.. -Iquirks -std=c++17 -W -Wall -O2 -fno-exceptions -fno-rtti -ffast-math -ffreestanding -fvisibility=hidden --target=wasm32
LDFLAGS = -nostdlib -Wl,--export-dynamic,--no-entry
QUIRKS = quirks/cmath quirks/cstdint quirks/cstdlib

.PHONY: test clean

test: simd_test_simd128.wasm simd_test_generic.wasm
	node simd_test.mjs

simd_test_simd128.wasm: simd_test.cc ../simd.hh ../simd128.hh ../simd128_double.hh ../simd128_triple.hh ../simd128_quadruple.hh $(QUIRKS)
	$(CXX) $(CXXFLAGS) -msimd128 $(LDFLAGS) $< -o $@

simd_test_generic.wasm: simd_test.cc ../simd.hh $(QUIRKS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f *.wasm

