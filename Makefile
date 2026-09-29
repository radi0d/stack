.PHONY: clean

CFLAGS ?= -g -O2 -std=c23 -Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wdouble-promotion -Wfloat-equal -Wformat=2 -Winline -Wlogical-op -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wno-unused-parameter -Wold-style-definition -Wpointer-arith -Wredundant-decls -Wrestrict -Wshadow -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wundef -Wwrite-strings -Wenum-conversion -fanalyzer -Iinclude/ -lm
CC ?= gcc

SRC = src/main.c src/stack.c
OBJ = $(patsubst src/%.c, %.o, $(SRC))
EXE = stack

all: $(EXE)

%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o $(EXE)
