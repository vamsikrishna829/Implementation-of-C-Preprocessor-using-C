CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L

TARGET = mypreprocessor

OBJS = main.o comment_removal.o macro_handler.o file_inclusion.o

$(TARGET): $(OBJS)
        gcc $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c preprocessor.h
        gcc $(CFLAGS) -c main.c

comment_removal.o: comment_removal.c preprocessor.h
        gcc $(CFLAGS) -c comment_removal.c

macro_handler.o: macro_handler.c preprocessor.h
        gcc $(CFLAGS) -c macro_handler.c

file_inclusion.o: file_inclusion.c preprocessor.h
        gcc $(CFLAGS) -c file_inclusion.c

clean:
        rm -f $(OBJS) $(TARGET) inputfile.i

.PHONY: clean
