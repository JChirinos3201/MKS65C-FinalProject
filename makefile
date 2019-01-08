all: client server

client: client.o pipe_networking.o helpers.o
	gcc -o client client.o pipe_networking.o helpers.o

server: server.o pipe_networking.o helpers.o
	gcc -o server server.o pipe_networking.o helpers.o

client.o: client.c pipe_networking.h
	gcc -c client.c

server.o: server.c pipe_networking.h
	gcc -c server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

helpers.o: helpers.c helpers.h
	gcc -c helpers.c

clean:
	-rm -f *.o *~ client server ClientFIFO Sesame /tmp/ClientFIFO /tmp/Sesame
