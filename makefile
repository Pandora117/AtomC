CC=gcc
CFLAGS=-Wall -Wextra -pedantic
# CFLAGS=-Wall -Wextra -pedantic -std=c99

AtomC: main.c
	$(CC) $(CFLAGS) main.c -o atomc