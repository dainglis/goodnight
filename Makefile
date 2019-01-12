
CC = gcc
CFLAGS = -Wall

make goodnight :
	$(CC) $(CFLAGS) ./src/*.c -o goodnight

