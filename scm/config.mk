CSCFLAGS += -O5

NAME = $(shell basename $(PWD))
BIN = bin/$(NAME)
SRC = src/$(NAME).scm

all : $(BIN)

$(BIN) : $(SRC)
	@mkdir -p bin
	csc $(CSCFLAGS) $< -o $@

clean :
	rm -rf bin

.PHONY : all clean
