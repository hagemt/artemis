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

.PHONY: all clean

$(LIB_STATIC): $(LIB_OBJECTS)
	@$(SAY) "AR" "$@"
	@$(AR) rcs "$@" $^

$(LIB_SHARED): $(LIB_OBJECTS)
	@$(SAY) "LD" "$@"
	@$(CC) $(CFLAGS) -shared $^ -o $@

%.o: %.c
	@$(SAY) "CC" "$<"
	@$(CC) $(CFLAGS) -c $< -o $@
