src_dir := ../src

vpath %.cpp $(src_dir)
vpath %.h $(src_dir)

objects := main.o board.o movegen.o position.o precompute.o test.o

engine: $(objects)
	g++ $^ -o $@

main.o: main.cpp
	g++ -c $< -o $@

board.o: board.cpp board.h
	g++ -c $< -o $@

movegen.o: movegen.cpp movegen.h
	g++ -c $< -o $@

position.o: position.cpp position.h
	g++ -c $< -o $@

precompute.o: precompute.cpp precompute.h
	g++ -c $< -o $@

test.o: test.cpp test.h
	g++ -c $< -o $@

clean:
	del /Q *.o

cleaner:
	del /Q *.o *.exe

.PHONY: clean cleaner