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
ASFLAGS	+= -felf64 -w+all -w-reloc-rel-dword
LDFLAGS	+=
LDLIBS	+= -lm


# Code dependencies

ffge.o:			ffge.h

bench-fullrank:		bench-fullrank.o ffge.o ffge_32i8.o

test-ffge:		test-ffge.o ffge.o



# Targets
.DEFAULT_GOAL := all

.PHONY: all \
	build build-bench build-test \
	clean debug \
	test

all: build build-test

debug: build
debug: CFLAGS	+= -DDEBUG -g -Og
debug: ASFLOGS	+= -DDEBUG -g -Fdwarf

PROGS	:= 
build: $(PROGS)

BENCH	:= bench-fullrank
build-bench: build $(BENCH)
build-bench: CFLAGS	+= -DBENCH -O3 -march=native -mavx2
build-bench: ASFLAGS	+= -DBENCH -Ox
build-bench: LDLIBS	+= -lflint

bench: build-bench 
	@for bb in $(BENCH); do 					\
		./$$bb && echo "$$bb: OK" || ( echo "$$bb: FAIL"; exit 1 ); \
	done


TEST	:= test-ffge
build-test: $(TEST)
build-test: CFLAGS	+= -DTEST -g -Og -march=native
build-test: ASFLAGS	+= -DTEST -g -Fdwarf

test: build-test
	@for tt in $(TEST); do						\
		./$$tt && echo "$$tt: OK" || ( echo "$$tt: FAIL"; exit 1 ); \
	done


clean:
	$(RM) *.o *.d
	$(RM) $(PROGS) $(BENCH) $(TEST)


