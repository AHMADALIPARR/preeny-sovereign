FC ?= gfortran
FFLAGS ?= -shared -fPIC -O2 -Wall -std=f2008

SRCS = $(wildcard *.f90)
SOS  = $(patsubst %.f90,%.so,$(SRCS))

all: $(SOS)

%.so: %.f90
	$(FC) $(FFLAGS) -o $@ $<

clean:
	rm -f *.so *.mod

.PHONY: all clean
