CC := gcc
CFLAGS := -lncurses -Wall -Wpedantic -march=znver3 -std=c2x

build_dir := ../build/
objects := $(build_dir)main.o $(build_dir)art.o $(build_dir)main-menu.o $(build_dir)quiz.o $(build_dir)actual-work.o

$(build_dir)self: $(objects)
	$(CC) $(CFLAGS) $^ -o $@

$(build_dir)main.o: main.c art.h main-menu.h quiz.h actual-work.h preprocessor.h
	$(CC) $(CFLAGS) -c $< -o $@

$(build_dir)art.o: art.c art.h
	$(CC) $(CFLAGS) -c $< -o $@

$(build_dir)main-menu.o: main-menu.c main-menu.h preprocessor.h
	$(CC) $(CFLAGS) -c $< -o $@

$(build_dir)quiz.o: quiz.c quiz.h preprocessor.h
	$(CC) $(CFLAGS) -c $< -o $@

$(build_dir)actual-work.o: actual-work.c actual-work.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(objects) $(build_dir)self

.PHONY: clean
