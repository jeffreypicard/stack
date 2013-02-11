AS=nasm -f elf64
CC=gcc -O3
DEBUG=-g
WARN=-Wall
CFLAGS=$(DEBUG) $(WARN)
LDFLAGS=-lpthread -lm

test_stack: stack.o push_pop.o test_stack.o
	$(CC) $(CFLAGS) $^ -o test_stack $(LDFLAGS)

test_asm: push_pop.o test_asm.o
	$(CC) $(CFLAGS) $^ -o test_asm $(LDFLAGS)

test_stack.o: test_stack.c
	$(CC) -c $(CFLAGS) $^

test_asm.o: test_asm.c
	$(CC) -c $(CFLAGS) $^

stack.o: stack.c stack.h
	$(CC) -c stack.c

push_pop.o: push_pop.asm
	$(AS) $^

.PHONY: clean

clean:
	-rm -rf push_pop.o test_asm.o test_stack.o test_asm test_stack
