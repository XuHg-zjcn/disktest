CC=gcc
LD=gcc
CFLAGS = -Wall -march=native -std=gnu99 -O3
LDFLAGS = -std=gnu99

INCS = -Iinclude -Ilibcrc/include -Ilibcrc/tab
SDIR = src

CSRC = $(shell find $(SDIR) -name *.c)
OBJF = $(CSRC:%.c=obj/%.o)

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(INCS) $(CFLAGS) -c -o "$@" "$<"

all: $(OBJF)
	@mkdir -p bin
	$(LD) $(LDFLAGS) -o bin/main $^

clean:
	rm -rf obj
	rm -rf bin
