
# Para compilacion

all: prodcons
LIBTHREAD = lpthread

.SUFFIXES:
.SUFFIXES: .o .c .s

.c.o .s.o:
	gcc -g -c $<

prodcons: prodcons.o
	gcc prodcons.o -o $@ -D_POSIX_C_SOURCE=199506L -$(LIBTHREAD) -lrt

clean:
	rm -f *.o *~ core
