all: graphics.cpp graphics.h main.cpp
	g++ main.cpp graphics.cpp -std=c++11 -I/usr/include/GL -lglut -lGL -lGLU -Ofast
	./a.out

gdb: graphics.cpp graphics.h main.cpp
	g++ main.cpp graphics.cpp -std=c++11 -g -I/usr/include/GL -lglut -lGL -lGLU -Ofast
	gdb ./a.out
