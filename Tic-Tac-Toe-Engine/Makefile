CXXFLAGS := -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

main: main.cpp
	g++ $^ -o $@ $(CXXFLAGS)

clean:
	del *.exe

.PHONY: clean
