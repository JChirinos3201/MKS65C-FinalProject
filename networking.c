// Joan Chirinos

#include "networking.h"

void error_check( int i, char *s ) {
  if ( i < 0 ) {
    printf("[%s] error %d: %s\n", s, errno, strerror(errno) );
    exit(1);
  }
}

/*=========================
  server_setup
  args:
  creates, binds a server side socket
  and sets it to the listening state
  returns the socket descriptor
  =========================*/
int server_setup() {
  int sd, i;

  //create the socket
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "server socket" );
  printf("[server] socket created\n");

  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;
  hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;  //IPv4 address
  hints->ai_socktype = SOCK_STREAM;  //TCP socket
  hints->ai_flags = AI_PASSIVE;  //Use all valid addresses
  getaddrinfo(NULL, PORT, hints, &results); //NULL means use local address

  //bind the socket to address and port
  i = bind( sd, results->ai_addr, results->ai_addrlen );
  error_check( i, "server bind" );
  printf("[server] socket bound\n");

  //set socket to listen state
  i = listen(sd, 10);
  error_check( i, "server listen" );
  printf("[server] socket in listen state\n");

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);
  return sd;
}

/*=========================
  server_connect
  args: int sd
  sd should refer to a socket in the listening state
  run the accept call
  returns the socket descriptor for the new socket connected
  to the client.
  =========================*/
int server_connect(int sd) {
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;

  sock_size = sizeof(client_address);
  client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size);
  error_check(client_socket, "server accept");

  return client_socket;
}

/*=========================
  client_setup
  args: int * to_server
  to_server is a string representing the server address
  create and connect a socket to a server socket that is
  in the listening state
  returns the file descriptor for the socket
  =========================*/
int client_setup(char * server) {
  int sd, i;

  //create the socket
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "client socket" );

  //run getaddrinfo
  /* hints->ai_flags not needed because the client
     specifies the desired address. */
  struct addrinfo * hints, * results;
  hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;  //IPv4
  hints->ai_socktype = SOCK_STREAM;  //TCP socket
  getaddrinfo(server, PORT, hints, &results);

  //connect to the server
  //connect will bind the socket for us
  i = connect( sd, results->ai_addr, results->ai_addrlen );
  error_check( i, "client connect" );

  free(hints);
  freeaddrinfo(results);

  return sd;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  int mkfifo_status = mkfifo(CLIENT_TO_SERVER, 0644);
  if (mkfifo_status == -1) {
    printf("MKFIFO SERVER BROKED: %s\n", strerror(errno));
    exit(1);
  }

  char* response = calloc(sizeof(char), 20);
  int downstream = open(CLIENT_TO_SERVER, O_RDONLY);
  read(downstream, response, 200);
  // printf("server got: |%s|\n", response);
  remove(CLIENT_TO_SERVER);

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

  int mkfifo_status = mkfifo(SERVER_TO_CLIENT, 0644);
  if (mkfifo_status == -1) {
    printf("MKFIFO CLIENT BROKED: %s\n", strerror(errno));
    exit(1);
  }

  int upstream = open(CLIENT_TO_SERVER, O_WRONLY);
  write(upstream, SERVER_TO_CLIENT, 20);
  // printf("client wrote to server\n");

  char* response = calloc(sizeof(char), 200);
  int downstream = open(SERVER_TO_CLIENT, O_RDONLY);
  read(downstream, response, 9);
  // printf("client got: |%s|\n", response);
  remove(SERVER_TO_CLIENT);

  write(upstream, "K, we're connected!", 20);

  *to_server = upstream;

  return downstream;
}
