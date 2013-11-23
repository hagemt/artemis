#
# This file is part of: libslf4c
#
# libslf4c is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# libslf4c is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with libslf4c. <http://www.gnu.org/licenses/>
#

TAG := libslf4c
XXX := DEBUG

AR  := $(shell which ar)
CC  := $(shell which gcc)
SAY := $(shell which echo) -e "[$(TAG)]"

CFLAGS_WARNING := -Wall -Wextra -Wno-long-long -Wno-variadic-macros
CFLAGS_DEBUG   := -ggdb -O0 -pedantic -DTAG="$(TAG)-dev-dbg"
CFLAGS_RELEASE := -DNDEBUG -O3 -ffast-math -DTAG="$(TAG)-dev"
CFLAGS         := -std=c99 -fPIC $(CFLAGS_WARNING) $(CFLAGS_$(XXX))

LIB_NAME    := slf4c
LIB_SOURCES := core.c function.c level.c
LIB_OBJECTS := $(LIB_SOURCES:.c=.o)
LIB_STATIC  := lib$(LIB_NAME).a
LIB_SHARED  := lib$(LIB_NAME).so

all: $(TAG) $(LIB_SHARED)

clean:
	@$(RM) -fv *.o $(TAG)*

.PHONY: all clean

$(TAG): loader.o $(LIB_STATIC)
	@$(SAY) "STATIC" "$@"
	@$(CC) $(CFLAGS) -L . -lslf4c -ldl $< -o $@

$(LIB_STATIC): $(LIB_OBJECTS)
	@$(SAY) "AR" "$@"
	@$(AR) rcs "$@" $^

$(LIB_SHARED): $(LIB_OBJECTS)
	@$(SAY) "LD" "$@"
	@$(CC) $(CFLAGS) -shared -ldl $^ -o $@

%.o: %.c
	@$(SAY) "CC" "$<"
	@$(CC) $(CFLAGS) -c $< -o $@
