CC=gcc
CFLAGS=-Wall -Werror -g
LDLIBS=-lpthread

all:	clean read_write_priority_writer read_write_priority_reader


read_write_priority_writer: read_write_priority_writer.c
	$(CC) $(CFLAGS) -o read_write_priority_writer read_write_priority_writer.c $(LDLIBS)

read_write_priority_reader: read_write_priority_reader.c
	$(CC) $(CFLAGS) -o read_write_priority_reader read_write_priority_reader.c $(LDLIBS)


clean:
	rm -f *.o
