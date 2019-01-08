#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define ACK "HOLA"

#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000

#define CLIENT_TO_SERVER "/tmp/Sesame"
#define SERVER_TO_CLIENT "/tmp/ClientFIFO"

struct deck {
  char** cards;
  size_t size;
  int card_at;
};

int server_handshake(int *to_client);

int client_handshake(int *to_server);

#endif
