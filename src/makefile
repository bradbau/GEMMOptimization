CC			= /opt/rh/devtoolset-7/root/usr/bin/gcc
CFLAGS		= -O3 -march=skylake-avx512 -w 
MKLPATH		= /opt/intel/mkl
LDFLAGS		= -L$(MKLPATH)/lib/intel64 -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl -DMKL_ILP64 -m64 
INCFLAGS	= -I$(MKLPATH)/include

#SRC = $( wildcard *.c)
SRC := $(shell find *.c ! -name "cblas.c")
HEADERS = $(wildcard *.h)

all: main cblas

main: $(SRC) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCFLAGS)  $(SRC) -o main

cblas: cblas.c utils.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCFLAGS)  cblas.c -o cblas

clean: 
	rm main cblas