CC := gcc
DEPS = main.c
CFLAGS := -nostdlib -ffreestanding -nostartfiles

all:
	$(CC) $(DEPS) $(CFLAGS) -fno-PIE -m32 -o test32
	$(CC) $(DEPS) -c $(CFLAGS) -fno-PIE -m32 -o test32.o
	$(CC) $(DEPS) $(CFLAGS) -m32 -o test32pie
	$(CC) $(DEPS) $(CFLAGS) -fPIE -m32 -o test32pic
	$(CC) $(DEPS) $(CFLAGS) -fno-PIE -m64 -o test64
	$(CC) $(DEPS) -c $(CFLAGS) -fno-PIE -m64 -o test64.o
	$(CC) $(DEPS) $(CFLAGS) -m64 -o test64pie
	$(CC) $(DEPS) $(CFLAGS) -fPIE -m64 -o test64pic

clean:
	@rm test32
	@rm test32.o
	@rm test32pic
	@rm test32pie
	@rm test64
	@rm test64.o
	@rm test64pic
	@rm test64pie
