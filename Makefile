objects=Exercise_2.o BTreeImplementation.o
Exercise_2 : $(objects)
	gcc $(objects) -o Exercise_2
Exercise_2.o :Exercise_2.c BTreeInterface.h BTreeTypes.h
	gcc -c Exercise_2.c
BTreeImplementation.o :BTreeImplementation.c BTreeInterface.h BTreeTypes.h
	gcc -c BTreeImplementation.c
clean:
	rm Exercise_2 $(objects)
