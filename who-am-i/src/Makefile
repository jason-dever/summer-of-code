build_dir := ../build/

CC := gcc
CFLAGS := -lncurses -Wall -Wpedantic -march=znver3 -std=c2x

objects := $(addprefix $(build_dir), main.o art.o main-menu.o quiz.o actual-work.o)

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

$(objects): | $(build_dir)

$(build_dir):
	mkdir $(build_dir)

clean:
	rm -rf $(build_dir)

.PHONY: clean
