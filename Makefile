AR := $(shell which ar)
CC := $(shell which gcc)
PC := $(shell which pkg-config)
RM := $(shell which rm) -fv

TAG  := hashset
TYPE := DEBUG

SAY := $(shell which echo) -e "[$(TAG)]"

CFLAGS_WARNING := -Wall -Wextra -Wno-long-long -Wno-variadic-macros
CFLAGS_DEBUG   := -ggdb -O0 -pedantic
CFLAGS_RELEASE := -DNDEBUG -O3 -ffast-math
CFLAGS         := -fPIC $(CFLAGS_WARNING) $(CFLAGS_$(TYPE))

LIB_NAME    := slf4c
LIB_SOURCES := loader.c log.c
LIB_OBJECTS := $(LIB_SOURCES:.c=.o)
LIB_STATIC  := lib$(LIB_NAME).a
LIB_SHARED  := lib$(LIB_SHARED).so

LDFLAGS := -shared -L .
LDLIBS  := -l$(LIB_NAME) $(shell $(PC) libcalg-1.0 --libs)
CFLAGS  := $(CFLAGS) $(shell $(PC) libcalg-1.0 --cflags)

all: aux $(TAG)

clean:
	@$(SAY) "cleaning object files"
	@$(RM) *.o *.obj
	@$(SAY) "cleaning libraries"
	@$(RM) $(LIB_STATIC) $(LIB_DYNAMIC)
	@$(SAY) "cleaning executables"
	@$(RM) $(TAG)

.PHONY: all clean

aux: bit.o uid.o

$(LIB_STATIC): $(LIB_OBJECTS)
	@$(SAY) "AR" "$@"
	@$(AR) rcs $@ $^

$(LIB_SHARED): $(LIB_OBJECTS)
	@$(SAY) "LD" "$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TAG): $(LIB_STATIC) crawl.o record.o main.o
	@$(SAY) "XX" "$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

%.o: %.c
	@$(SAY) "CC" "$<"
	@$(CC) $(CFLAGS) -c $< -o $@
