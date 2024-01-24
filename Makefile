main: ./src/*.c
	clang -o build/main src/*.c `sdl2-config --cflags --libs`