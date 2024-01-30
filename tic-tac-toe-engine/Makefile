CXXFLAGS := -march=znver3 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

main: main.cpp
	g++ $^ -o $@ $(CXXFLAGS)
clean:
	rm -f main

.PHONY: clean
