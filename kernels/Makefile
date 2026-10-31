FC ?= gfortran
FFLAGS ?= -shared -fPIC -O2 -Wall -std=f2008

all: libsovereign_math.so libsovereign_vector.so

libsovereign_math.so: sovereign_math.f90
	$(FC) $(FFLAGS) -o $@ $<

libsovereign_vector.so: sovereign_vector.f90
	$(FC) $(FFLAGS) -o $@ $<

clean:
	rm -f *.so *.mod

.PHONY: all clean
