all: server.o client.o networking.o helpers.o
	gcc -o client_run client.o networking.o helpers.o
	gcc -o server_run server.o networking.o helpers.o

server:
	./server_run

client:
	./client_run $(args)

client.o: client.c networking.h
	gcc -c client.c

server.o: server.c networking.h
	gcc -c server.c

networking.o: networking.c networking.h
	gcc -c networking.c

helpers.o: helpers.c helpers.h
	gcc -c helpers.c

clean:
	-rm -f *.o *~ client_run server_run ClientFIFO Sesame /tmp/ClientFIFO /tmp/Sesame
