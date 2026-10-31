CC ?= gcc
CFLAGS_STUBS ?= -shared -fPIC -Wall -O2
FC ?= gfortran
FFLAGS ?= -shared -fPIC -O2 -Wall -std=f2008

C_SRCS = $(wildcard src/*.c)
C_SOS  = $(patsubst src/%.c,%.so,$(C_SRCS))

all: stubs kernels os kernel kernel64

stubs: $(C_SOS)

kernels:
	$(MAKE) -C kernels

os: kernels
	$(MAKE) -C os

kernel:
	$(MAKE) -C kernel

kernel64:
	$(MAKE) -C kernel64

iso:
	$(MAKE) -C kernel iso

%.so: src/%.c
	$(CC) $(CFLAGS_STUBS) -o $@ $<

clean:
	rm -f *.so
	$(MAKE) -C kernels clean
	$(MAKE) -C os clean
	$(MAKE) -C kernel clean
	$(MAKE) -C kernel64 clean

.PHONY: all stubs kernels os kernel kernel64 iso clean
