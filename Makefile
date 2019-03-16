gconv: gconv.o
	cc -o gconv -lmd gconv.o
	
gconv.o:
	cc -c  -I/usr/include -I/sys gconv.c
	
clean:
	rm gconv gconv.o


