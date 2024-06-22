# --------------------------------------------------------------------------- #
# Copyright 2024 ⧉⧉⧉                                                          #
#                                                                             #
# This program is free software: you can redistribute it and/or modify it     #
# under the terms of the GNU General Public License as published by the       #
# Free Software Foundation, either version 3 of the License, or (at your      #
# option) any later version.                                                  #
#                                                                             #
# This program is distributed in the hope that it will be useful, but         #
# WITHOUT ANY WARRANTY# without even the implied warranty of MERCHANTABILITY  #
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    #
# for more details.                                                           #
#                                                                             #
# You should have received a copy of the GNU General Public License along     #
# with this program.  If not, see <https://www.gnu.org/licenses/>.            #
# --------------------------------------------------------------------------- #

CC	?= gcc
CFLAGS	+= -std=c11 -Wall -Wextra -O2
AS	:= nasm
ASFLAGS	+= -Ox -felf64 -w+all -w-reloc-rel-dword
LDFLAGS	+=
LDLIBS	+= -lm


FLINT_PREFIX	:= $(HOME)/usr
FLINT_INCLUDE	= -I$(FLINT_PREFIX)/include/flint
FLINT_LDFLAGS	= -L$(FLINT_PREFIX)/lib -Wl,-rpath -Wl,$(FLINT_PREFIX)/lib
FLINT_LDLIBS	= -lflint


CFLAGS	+= $(FLINT_INCLUDE)
LDFLAGS	+= $(FLINT_LDFLAGS)
LDLIBS	+= $(FLINT_LDLIBS)

# Code dependencies

bench-ffge: bench-ffge.o


# Targets

.PHONY: all build clean debug
.DEFAULT_GOAL	:= all

all: build

debug: build
debug: CFLAGS	+= -DDEBUG -g -Og
debug: ASFLOGS	+= -DDEBUG -g -Fdwarf

PROGS	:= bench-ffge
BENCH	:= bench-ffge

build: $(PROGS)

bench: CFLAGS	+= -DBENCH -O3 -march=native -mavx2
bench: ASFLAGS	+= -DBENCH
bench: build
	@for bb in $(BENCH); do 					\
		./$$bb && echo "  $$bb: OK" || ( echo "  $$bb: FAIL" );	\
	done

clean:
	$(RM) *.o *.d
	$(RM) $(PROGS)

