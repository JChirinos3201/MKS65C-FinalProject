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

  char * client_to_server = "Sesame";

  int mkfifo_status = mkfifo(client_to_server, 0644);
  if (mkfifo_status == -1) {
    printf("MKFIFO SERVER BROKED: %s\n", strerror(errno));
    exit(1);
  }

  char* response = calloc(sizeof(char), 20);
  int downstream = open(client_to_server, O_RDONLY);
  read(downstream, response, 200);
  // printf("server got: |%s|\n", response);
  remove(client_to_server);

  int upstream = open(response, O_WRONLY);
  write(upstream, "Got it!", 8);
  // printf("server wrote to client\n");

  read(downstream, response, 200);
  // printf("server got: |%s|\n", response);

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

  char * client_to_server = "Sesame";
  char * server_to_client = "ClientFIFO";

  int mkfifo_status = mkfifo(server_to_client, 0644);
  if (mkfifo_status == -1) {
    printf("MKFIFO CLIENT BROKED: %s\n", strerror(errno));
    exit(1);
  }

  int upstream = open(client_to_server, O_WRONLY);
  write(upstream, server_to_client, 20);
  // printf("client wrote to server\n");

  char* response = calloc(sizeof(char), 200);
  int downstream = open(server_to_client, O_RDONLY);
  read(downstream, response, 9);
  // printf("client got: |%s|\n", response);
  remove(server_to_client);

  write(upstream, "K, we're connected!", 20);

  *to_server = upstream;

  return downstream;
}
