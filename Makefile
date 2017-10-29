CC=gcc
CFLAGS=-Wall -Werror -g
LDLIBS=-lpthread

all: clean read_write_priority_writer read_write_priority_reader read_write_priority_fifo_v2


read_write_priority_writer: read_write_priority_writer.c
	$(CC) $(CFLAGS) -o read_write_priority_writer read_write_priority_writer.c $(LDLIBS)

read_write_priority_reader: read_write_priority_reader.c
	$(CC) $(CFLAGS) -o read_write_priority_reader read_write_priority_reader.c $(LDLIBS)

read_write_priority_fifo_v2: read_write_priority_fifo_v2.c
	$(CC) $(CFLAGS) -o read_write_priority_fifo read_write_priority_fifo_v2.c $(LDLIBS)

clean:
	rm -f *.o
