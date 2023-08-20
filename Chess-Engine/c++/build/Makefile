src_dir := ../src

vpath %.cpp $(src_dir)
vpath %.hpp $(src_dir)

CXX := g++
CXXFLAGS := -Wall -Werror -Wpedantic -O0 -march=znver3

objects := main.o board.o position.o precompute.o test.o

engine: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp board.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

board.o: board.cpp board.hpp movegen.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

position.o: position.cpp position.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

precompute.o: precompute.cpp precompute.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test.o: test.cpp test.hpp board.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o engine

.PHONY: clean