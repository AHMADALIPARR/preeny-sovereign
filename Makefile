CC ?= gcc
CFLAGS ?= -shared -fPIC -Wall -O2
FC ?= gfortran
FFLAGS ?= -shared -fPIC -O2 -Wall -std=f2008

C_SRCS = $(wildcard src/*.c)
C_SOS  = $(patsubst src/%.c,%.so,$(C_SRCS))

F_SRCS = $(wildcard kernels/*.f90)
F_SOS  = $(patsubst kernels/%.f90,kernels/%.so,$(F_SRCS))

all: stubs kernels

stubs: $(C_SOS)

kernels: $(F_SOS)

%.so: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

kernels/%.so: kernels/%.f90
	$(FC) $(FFLAGS) -o $@ $<

clean:
	rm -f *.so kernels/*.so kernels/*.mod

.PHONY: all stubs kernels clean
