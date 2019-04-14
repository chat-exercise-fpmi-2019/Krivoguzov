CC=gcc
BUILDDIR=build
CFLAGS=

all: server client

server: server.o clientList.o
	$(CC) $(CFLAGS) server.o clientList.o -o server

server.o: server.c server.h 
	$(CC) $(CFLAGS) -c server.c

client: client.o
	$(CC) $(CFLAGS) client.o -o client

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clientList.o: clientList.c
	$(CC) $(CFLAGS) -c clientList.c

clean:
	rm -rf *.o *.so server client
