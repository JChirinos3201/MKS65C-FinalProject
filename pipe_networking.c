// Joan Chirinos

#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  int mkfifo_status = mkfifo("Sesame", 0644);
  if (mkfifo_status == -1) {
    printf("MKFIFO SERVER BROKED: %s\n", strerror(errno));
    exit(1);
  }

  char* response = calloc(sizeof(char), 10);
  int downstream = open("Sesame", O_RDONLY);
  read(downstream, response, 200);
  printf("server got: |%s|\n", response);
  remove("Sesame");

  int upstream = open(response, O_WRONLY);
  write(upstream, "Got it!", 8);
  printf("server wrote to client\n");

  read(downstream, response, 200);
  printf("server got: |%s|\n", response);

  *to_client = upstream;

  return downstream;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

  int mkfifo_status = mkfifo("ClientFIFO", 0644);
  if (mkfifo_status == -1) {
    printf("MKFIFO CLIENT BROKED: %s\n", strerror(errno));
    exit(1);
  }

  int upstream = open("Sesame", O_WRONLY);
  write(upstream, "ClientFIFO", 11);
  printf("client wrote to server\n");

  char* response = calloc(sizeof(char), 200);
  int downstream = open("ClientFIFO", O_RDONLY);
  read(downstream, response, 9);
  printf("client got: |%s|\n", response);
  remove("ClientFIFO");

  write(upstream, "K, we're connected!", 20);

  *to_server = upstream;

  return downstream;
}
