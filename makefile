tetris: main.o layout.o shape.o 
	gcc -g -o tetris main.o layout.o shape.o -lraylib -lm
main.o: main.c layout.h shape.h
	gcc -c main.c
layout.o: layout.c layout.h
	gcc -c layout.c
shape.o: shape.c shape.h
	gcc -c shape.c 
clean:
	rm tetris main.o layout.o shape.o 
