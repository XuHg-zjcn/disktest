CC=gcc
LD=gcc
CFLAGS = -Wall -march=native -std=gnu99 -O3
LDFLAGS = -lbsd

INCS = -Iinclude -Ilibcrc/include -Ilibcrc/tab
SDIR = src

CSRC = $(shell find $(SDIR) -name *.c)
OBJF = $(CSRC:%.c=obj/%.o)

all: bin/main

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(INCS) $(CFLAGS) -c -o "$@" "$<"

bin/main: $(OBJF)
	@mkdir -p bin
	$(LD) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf obj
	rm -rf bin
