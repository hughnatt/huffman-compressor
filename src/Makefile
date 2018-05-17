CC=gcc
CFLAGS=-Wall -Werror -g

SRCS=io.c huffman.c codage.c analyse.c
OBJS=$(SRCS:.c=.o)

INCDIR=../include/
CFLAGS+=-I../include/

EXEC=compression decompression

all: $(EXEC)

%.o : %.c $(INCDIR)%.h
	$(CC) $(CFLAGS) -c $< -I$(INCDIR)

codage.o : codage.c $(INCDIR)codage.h $(INCDIR)huffman.h
	$(CC) $(CFLAGS) -c $< -I$(INCDIR)

io.o : io.c $(INCDIR)io.h $(INCDIR)huffman.h
	$(CC) $(CFLAGS) -c $< -I$(INCDIR)

compression.o: compression.c $(INCDIR)huffman.h $(INCDIR)codage.h $(INCDIR)analyse.h $(INCDIR)io.h
	$(CC) $(CFLAGS) -c $< -I$(INCDIR)

decompression.o: decompression.c $(INCDIR)io.h
	$(CC) $(CFLAGS) -c $< -I$(INCDIR)

compression: compression.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

decompression: decompression.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJS) $(EXEC) *.o


