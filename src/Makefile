CC=gcc
CFLAGS=-Wall -Werror -g

SRCS=io.c huffman.c codage.c analyse.c
OBJS=$(OBJS:.c=.o)

INCDIR=../include/
CFLAGS+=-I../include/

EXEC=compression decompression


%.o : %.c $(INCDIR)%.h
	$(CC) $(CFLAGS) -c $<

compression.o: compression.c
	$(CC) $(CFLAGS) -c $<

decompression.o: decompression.c
	$(CC) $(CFLAGS) -c $<

compression: compression.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

decompression: decompression.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^	


