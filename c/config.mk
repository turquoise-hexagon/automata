CC     ?= gcc
CFLAGS += -pedantic -Wall -Wextra

NAME = $(shell basename $(PWD))
BIN  = bin/$(NAME)
SRC  = src/$(NAME).c

all : $(BIN)

$(BIN) : $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

clean :
	rm -rf bin

.PHONY : all clean
