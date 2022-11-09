CC=cc
CFLAGS=-g

test_list: hstk.o hstk_list.o test_list.o
	$(CC) $(CFLAGS) -o $@ $^

all: test_list

clean: rm *.o test_list

