
CC=gcc

emlimport : critbit.o nocr.o emlimport.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)
