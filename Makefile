CC ?= gcc
CFLAGS ?= -shared -fPIC -Wall -O2

SRCS = $(wildcard src/*.c)
SOS  = $(patsubst src/%.c,%.so,$(SRCS))

all: $(SOS)

%.so: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.so

.PHONY: all clean
