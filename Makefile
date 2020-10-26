OBJS	= main.o mouselist.o
SOURCE	= main.c mouselist.c
HEADER	= mouselist.h miscellaneous.h
OUT	= ghub-gui
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lusb-1.0

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c

singlylinkedlist.o: mouselist.c
	$(CC) $(FLAGS) mouselist.c


clean:
	rm -f $(OBJS) $(OUT)
