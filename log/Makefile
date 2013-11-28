TAG := libslf4c
XXX := DEBUG

AR  := $(shell which ar)
CC  := $(shell which gcc)
SAY := $(shell which echo) -e "[$(TAG)]"

# Build options (natural choices, totally sane)
CFLAGS_WARNING := -Wall -Wextra -Wno-long-long -Wno-variadic-macros
CFLAGS := -ansi -D_GNU_SOURCE $(CFLAGS_WARNING) $(CFLAGS)
# Build options (experimental, still mostly sane)
CFLAGS_DEBUG   := -ggdb -O0 -pedantic -DTAG="$(TAG)-dev-dbg"
CFLAGS_RELEASE := -DNDEBUG -O3 -ffast-math -DTAG="$(TAG)-dev"
CFLAGS := -fPIC $(CFLAGS_$(XXX)) $(CFLAGS)

LIB_NAME    := slf4c
LIB_SOURCES := $(wildcard *.c)
LIB_OBJECTS := $(LIB_SOURCES:.c=.o)
LIB_STATIC  := lib$(LIB_NAME).a
LIB_SHARED  := lib$(LIB_NAME).so

all: $(LIB_STATIC) $(LIB_SHARED)

clean:
	@$(SAY) "cleaning object files"
	@$(RM) -fv $(LIB_OBJECTS)
	@$(SAY) "cleaning library files"
	@$(RM) -fv $(LIB_STATIC) $(LIB_SHARED)
	@$(SAY) "cleaning test files"
	@$(RM) -fv test/*.o test/*.so test/functions test/size

test: test/libtest.so test/libslf4c.so test/functions test/size

.PHONY: all clean test

test/size: $(LIB_SHARED) test/size.o
	@$(SAY) "$(LIB_SHARED)" "->" "$@"
	@$(CC) $(CFLAGS) -L . -lslf4c test/size.o -o $@ -ldl

test/libslf4c.so: $(LIB_SHARED)
	@$(SAY) "../$(LIB_SHARED)" "->" "test/$(LIB_SHARED)"
	@ln -s "../$(LIB_SHARED)" "test/$(LIB_SHARED)"

test/libtest.so: test/libtest.o
	@$(SAY) "LD" "$@"
	@$(CC) $(CFLAGS) -shared $^ -o $@

test/functions: test/test.functions.o
	@$(SAY) "$(LIB_SHARED)" "->" "$@"
	@$(CC) $(CFLAGS) -L . -lslf4c $< -o $@ -ldl

$(LIB_STATIC): $(LIB_OBJECTS)
	@$(SAY) "AR" "$@"
	@$(AR) rcs "$@" $^

$(LIB_SHARED): $(LIB_OBJECTS)
	@$(SAY) "LD" "$@"
	@$(CC) $(CFLAGS) -shared $^ -o $@

%.o: %.c
	@$(SAY) "CC" "$<"
	@$(CC) $(CFLAGS) -I . -c $< -o $@
