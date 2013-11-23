TAG := artemis
SAY := $(shell which echo) -e "[$(TAG)]"

CFLAGS_INCLUDE := -I include $(pkg-config libcalg-1.0 --cflags)
CFLAGS_LIBRARY := -L . -L log $(pkg-config libcalg-1.0 --libs)
CFLAGS_WARNING := -Wall -Wextra -Wno-long-long -Wno-variadic-macros
CFLAGS := $(CFLAGS_INCLUDE) $(CFLAGS_LIBRARY) $(CFLAGS_WARNING) $(CFLAGS)
CFLAGS_DEBUG   := -ggdb -O0 -pedantic
CFLAGS_RELEASE := -DNDEBUG -O3 -ffast-math
CFLAGS := -std=ansi -D_GNU_SOURCE -fPIC $(CFLAGS_DEBUG) $(CFLAGS)

LIB_NAME    := artemis
LIB_SOURCES := core/*.c
LIB_OBJECTS := $(LIB_SOURCES:.c=.o)
LIB_SHARED  := lib$(LIB_NAME).so

all: $(TAG)

clean:
	@$(SAY) "cleaning object files"
	@$(RM) -fv core/*.o test/*.o
	@$(SAY) "cleaning libraries"
	@$(RM) -fv $(LIB_SHARED)
	@$(SAY) "cleaning executables"
	@$(RM) -fv $(TAG)

.PHONY: all clean

$(LIB_SHARED): $(LIB_OBJECTS)
	@$(SAY) "LD" "$@"
	@$(CC) $(CFLAGS) -shared $^ -o $@

$(TAG): $(LIB_SHARED) test/libartemis.o test/main.o
	@$(SAY) "XX" "$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

%.o: %.c
	@$(SAY) "CC" "$<"
	@$(CC) $(CFLAGS) -c $< -o $@
