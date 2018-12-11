
CXXFLAGS = -I.. -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native
CXX = clang++ -stdlib=libc++
#CXX = g++

.PHONY: clean test

tests := $(basename $(wildcard *_test.*))

test: $(tests)
	$(patsubst %,./%>/dev/null;,$(tests))

clean:
	rm -f *_test

