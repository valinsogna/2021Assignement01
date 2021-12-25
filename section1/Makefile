SRC = ring.c

CXX = mpicc

EXE = $(SRC:.c=.x)

# eliminate default suffixes
.SUFFIXES:
SUFFIXES =

# just consider our own suffixes
.SUFFIXES: .c .x

all: $(EXE)

.PHONY: all

%.x: %.c 
	$(CXX) $< -o $@ -lm

clean:
	rm -f $(EXE) *~

.PHONY: clean

clean_ring:
	rm -rf default ||:
	rm -rf by_core ||:
	rm -rf by_socket ||:
	rm -rf by_node ||:
	find . -type f -name ring.sh.\* -delete ||:

.PHONY: clean_ring


