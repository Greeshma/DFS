all: dserver dclient

clean:
	rm -f *.o server client dserver dclient

dserver: server.o common.o dummy_mod.o list.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	cp $@ $(INSTALLDIR)
	@echo -e "[Server linked]\n"

dclient: client.o corefs.o common.o dummy_mod.o list.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	cp $@ $(INSTALLDIR)
	@echo -e "[Client linked]\n"
