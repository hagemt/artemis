DEBUG_FLAGS := -ggdb -O0 -pedantic
RELEASE_FLAGS := -DNDEBUG -O3 -ffast-math

CC := $(shell which gcc)
CFLAGS := -Wall -Wextra $(DEBUG_FLAGS)

all: set

clean:
	$(RM) -v *.o
	$(RM) -v set

.PHONY: all clean

set: set.o
	$(CC) $(CFLAGS) -lcalg $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
