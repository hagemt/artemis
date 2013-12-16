TAG := tpool
SAY := $(shell which echo) -e "[$(TAG)]"

CFLAGS_INCLUDE := -nostdinc -I include -I ../libc/include -I /opt/include
CFLAGS_INCLUDE := $(CFLAGS_INCLUDE) $(shell pkg-config libcalg-1.0 --cflags)
CFLAGS_LIBRARY := -nostdlib -L ../libc/lib -L /opt/lib64 -lc -lgcc -lpthread
CFLAGS_LIBRARY := $(CFLAGS_LIBRARY) $(shell pkg-config libcalg-1.0 --libs)

CFLAGS_WARNING := -Wall -Wextra -Wno-long-long -Wno-variadic-macros
CFLAGS := $(CFLAGS_INCLUDE) $(CFLAGS_LIBRARY) $(CFLAGS_WARNING) $(CFLAGS)

CFLAGS_DEBUG   := -ggdb -O0 -pedantic
CFLAGS_RELEASE := -DNDEBUG -O3 -ffast-math
CFLAGS := -ansi -D_GNU_SOURCE -fPIC $(CFLAGS_DEBUG) $(CFLAGS)

LIB_NAME    := $(TAG)
LIB_SOURCES := $(wildcard *.c bits/*.c mapper/*.c runner/*.c)
LIB_OBJECTS := $(LIB_SOURCES:.c=.o)
LIB_SHARED  := lib$(LIB_NAME).so

all: $(LIB_SHARED)

clean:
	@$(SAY) "cleaning object files"
	@$(RM) -fv $(LIB_OBJECTS)
	@$(SAY) "cleaning libraries"
	@$(RM) -fv $(LIB_SHARED)

.PHONY: all clean

$(LIB_SHARED): $(LIB_OBJECTS)
	@$(SAY) "LD" "$@"
	@$(CC) $(CFLAGS) -shared $^ -o "$@"

%.o: %.c
	@$(SAY) "CC" "$<"
	@$(CC) $(CFLAGS) -c "$<" -o "$@"
