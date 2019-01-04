/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

/*
MVP

No settings; predefined values
*/

#include "pipe_networking.h"
#include "helpers.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("Sesame");
    printf("\n");
    exit(EXIT_SUCCESS);
  }
}

int MAX_PLAYER_COUNT = 3;
int SCORE_CAP = 10;

int main() {

  // set ctrl-c signal
  signal(SIGINT, sighandler);

  // array of black and white cards
  char ** black_cards = get_black_cards();
  char ** white_cards = get_white_cards();

  // array of file descriptors for pipes to/from clients
  int* to_client = calloc(sizeof(int), MAX_PLAYER_COUNT);
  int* from_client = calloc(sizeof(int), MAX_PLAYER_COUNT);

  // populate fd arrays
  int player_count = 0;
  while (player_count < MAX_PLAYER_COUNT) {
    from_client[player_count] = server_handshake(to_client + player_count);
    player_count++;
  }
  printf("all clients connected\n");

  // broadcasting example
  int i;
  for (i = 0; i < 3; i++) {
    char* t = calloc(sizeof(char), 2);
    sprintf(t, "%d", i);
    write(to_client[i], t, 14);
    free(t);
  }

}
