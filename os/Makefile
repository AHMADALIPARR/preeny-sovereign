CC ?= gcc
CFLAGS ?= -Wall -O2 -std=c11
LDFLAGS ?= -L../kernels -Wl,-rpath,../kernels

all: sovereign-init

sovereign-init: init.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -lsovereign_math

clean:
	rm -f sovereign-init

.PHONY: all clean
