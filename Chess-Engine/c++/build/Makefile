src_dir := ../src

vpath %.cpp $(src_dir)
vpath %.h $(src_dir)

CXX := g++
CXXFLAGS := -Wall -Werror -Wpedantic -O0 -march=znver3

objects := main.o board.o position.o precompute.o test.o

engine: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

board.o: board.cpp board.h movegen.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

position.o: position.cpp position.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

precompute.o: precompute.cpp precompute.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test.o: test.cpp test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q *.o

cleaner:
	del /Q *.o *.exe

.PHONY: clean cleaner