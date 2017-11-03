CC=gcc
CFLAGS=-Wall -Werror -std=c99

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
AOUT = q4 q5 q6 q7 q8

all : $(AOUT) 

prog : $(OBJS)
	@$(CC) $(LDFLAGS) -o $@ $^
%.o : %.c
	@$(CC) $(CFLAGS) -c $^

clean:
	@rm -rf *.o 

mrproper: clean
	@rm -rf q4 q5 q6 q7 q8
