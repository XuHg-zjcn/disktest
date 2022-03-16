CC=gcc
LD=gcc
CFLAGS = -Wall -std=gnu99
LDFLAGS = -std=gnu99

INCS = -Iinclude
SDIR = src

CSRC = $(shell find $(SDIR) -name *.c)
OBJF = $(CSRC:%.c=obj/%.o)

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(INCS) $(CFLAGS) -c -o "$@" "$<"

all: $(OBJF)
	@mkdir -p bin
	$(LD) $(LDFLAGS) -o obj/main $^

clean:
	rm -rf obj
	rm -rf bin
