out := ../build/

CXX := g++
CXXFLAGS := -Wall -Werror -Wpedantic -O3 -march=znver3 -Warray-bounds=0

objects := $(addprefix $(out), main.o board.o position.o precompute.o test.o)

$(out)engine: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(out)main.o: main.cpp board.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(out)board.o: board.cpp board.hpp movegen.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(out)position.o: position.cpp position.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(out)precompute.o: precompute.cpp precompute.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(out)test.o: test.cpp test.hpp board.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(objects): | $(out)

$(out):
	mkdir $(out)

clean:
	rm -rf $(out)

.PHONY: clean
