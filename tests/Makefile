
CXXFLAGS = -I.. -std=c++17 -W -Wall -O2 -fno-exceptions -fno-rtti -ffast-math -ftree-vectorize

CXX = clang++ -stdlib=libc++ -march=native

#CXX = g++ -march=native

#CXX = armv7a-hardfloat-linux-gnueabi-g++ -static -mfpu=neon -march=armv7-a
#QEMU = qemu-arm

.PHONY: clean test

tests := $(basename $(wildcard *_test.*))

test: $(tests)
	$(patsubst %,$(QEMU) ./% >/dev/null;,$(tests))

clean:
	rm -f *_test

