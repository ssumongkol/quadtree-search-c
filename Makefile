# define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# define libraries to be linked (for example -lm)
LIB = 

# define sets of header source files and object files
SRC3 = dict3.c data.c quadtree.c
SRC4 = dict4.c data.c quadtree.c array.c
# OBJ is the same as SRC, just replace .c with .h
OBJ3 = $(SRC3:.c=.o)
OBJ4 = $(SRC4:.c=.o)

# executable target
EXE3 = dict3
EXE4 = dict4

#the first target:
dict3: dict3.o quadtree.o data.o
	$(CC) $(CFLAGS) -o $(EXE3) $(OBJ3) $(LIB)

dict4: dict4.o array.o quadtree.o data.o
	$(CC) $(CFLAGS) -o $(EXE4) $(OBJ4) $(LIB)

dict3.o: dict3.c data.h quadtree.h

dict4.o: dict4.c data.h quadtree.h

array.o: array.c array.h quadtree.h data.h

quadtree.o: quadtree.c quadtree.h data.h

data.o: data.c data.h

clean:
	rm -f $(OBJ) $(EXE1) $(EXE2)