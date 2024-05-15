all: run

build: clean
	@mkdir -p obj
	@gcc -c libs/backend.c -o obj/backend.o
	@gcc -c libs/frontend.c -o obj/frontend.o
	@gcc -c libs/matrix.c -o obj/matrix.o
	@gcc -c libs/draw.c -o obj/draw.o
	@gcc -o build/tetris tetris.c obj/backend.o obj/matrix.o obj/draw.o obj/frontend.o -lncurses

run: build
	@clear
	@./build/tetris

valgrind: build
	@valgrind ./build/tetris

clang_format:
	@clang-format -n *.c
	@clang-format -n includes/*.h
	@clang_format -n libs/*.c

clean:
	@rm -rf obj
	@rm -rf build/tetris
