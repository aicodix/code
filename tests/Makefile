
CXXFLAGS = -I.. -std=c++17 -W -Wall -Ofast -fno-exceptions -fno-rtti

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

