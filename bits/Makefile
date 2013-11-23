TAG := bits
SAY := $(shell which echo) -e "[$(TAG)]"

CFLAGS_INCLUDE := -I ../log
CFLAGS_LIBRARY := -L ../log
CFLAGS_WARNING := -Wall -Wextra -Wno-long-long -Wno-variadic-macros
CFLAGS := $(CFLAGS_INCLUDE) $(CFLAGS_LIBRARY) $(CFLAGS_WARNING) $(CFLAGS)

CFLAGS_DEBUG   := -ggdb -O0 -pedantic
CFLAGS_RELEASE := -DNDEBUG -O3 -ffast-math

CFLAGS  := -ansi -D_GNU_SOURCE -fPIC $(CFLAGS_DEBUG) $(CFLAGS)
TARGETS := bits inc replib uid

all: $(TARGETS)

clean:
	@$(SAY) "cleaning built files"
	@$(RM) -fv *.o $(TARGETS)

.PHONY: all clean

bits: bit.o two.o
	@$(SAY) "LD" "$^"
	@$(CC) $(CFLAGS) $^ -o "$@"

inc: range.o
	@$(SAY) "LD" "$<"
	@$(CC) $(CFLAGS) $< -o "$@"

replib: dl.o
	@$(SAY) "LD" "$<"
	@$(CC) $(CFLAGS) -ldl -lreadline $< -o "$@"

uid: uid.o
	@$(SAY) "LD" "$<"
	@$(CC) $(CFLAGS) $< -o "$@"

%.o: %.c
	@$(SAY) "CC" "$<"
	@$(CC) $(CFLAGS) -c $< -o "$@"
