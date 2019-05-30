OBJS	= main.o Grid.o Item.o Living.o Spell.o
SOURCE	= main.cpp Grid.cpp Item.cpp Living.cpp Spell.cpp
HEADER	= Grid.h Item.h Living.h Spell.h
OUT	= game
CC	= g++
FLAGS	= -g -c

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@


#create-compile individual files.
main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

Grid.o: Grid.cpp
	$(CC) $(FLAGS) Grid.cpp

Item.o: Item.cpp
	$(CC) $(FLAGS) Item.cpp

Living.o: Living.cpp
	$(CC) $(FLAGS) Living.cpp

Spell.o: Spell.cpp
	$(CC) $(FLAGS) Spell.cpp


#clean place
clean:
	rm -f $(OBJS) $(OUT)

#acounting
count:
	wc $(SOURCE) $(HEADER)
