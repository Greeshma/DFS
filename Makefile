CC=gcc
LDFLAGS=-lconfig -lpthread 

all: server client

clean:
	rm -f *.o

server: server.o mem_server.o list.o
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo -e "[Server linked]\n"

client: client.o
	$(CC) $^ -o $@
	@echo -e "[Client linked]\n"
