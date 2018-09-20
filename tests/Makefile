
CXXFLAGS = -I.. -std=c++17 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native
CXX = clang++ -stdlib=libc++
#CXX = g++

.PHONY: clean test

tests := $(basename $(wildcard *_test.*))

test: $(tests)
	$(patsubst %,./%;,$(tests))

clean:
	rm -f *_test

