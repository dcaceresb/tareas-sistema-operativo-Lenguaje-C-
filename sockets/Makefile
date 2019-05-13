#
all: clitcp servtcp

.SUFFIXES:
.SUFFIXES: .o .c

.c.o .s.o:
	gcc -c $<

clitcp: clitcp.o 
	gcc -o $@ clitcp.o

servtcp: servtcp.o
	gcc -o $@ servtcp.o

clean:
	rm  -f *.o *~
