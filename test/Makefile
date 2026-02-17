CC := gcc
DEPS = main.c
CFLAGS := -nostdlib -ffreestanding -fno-PIE -nostartfiles

all:
	$(CC) $(DEPS) $(CFLAGS) -m32 -o test32
	$(CC) $(DEPS) $(CFLAGS) -m32 -o test32.o
	$(CC) $(DEPS) $(CFLAGS) -m64 -o test64
	$(CC) $(DEPS) $(CFLAGS) -m64 -o test64.o

clean:
	@rm test32
	@rm test32.o
	@rm test64
	@rm test64.o
