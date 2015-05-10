CC=gcc
CFLAGS=`pkg-config gtk+-2.0 --cflags` -Wall
LDFLAGS=`pkg-config gtk+-2.0 --libs`
EXEC=sudocool

compile: *.c
	$(CC) $(CFLAGS) $^ -o $(EXEC)  $(LDFLAGS)
clean:
		rm -rf *.o $(EXEC)
