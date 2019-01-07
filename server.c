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
    remove("/tmp/Sesame");
    printf("\n");
    exit(EXIT_SUCCESS);
  }
}

int MAX_PLAYER_COUNT = 3;
int SCORE_CAP = 10;

int main() {
  // seeding rand
  srand(time(NULL));

  // set ctrl-c signal
  signal(SIGINT, sighandler);

  // decks of black and white cards
  struct deck* black_deck = get_black_deck();
  struct deck* white_deck = get_white_deck();

  // shuffle decks
  shuffle(black_deck);
  shuffle(white_deck);

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

  // broadcasting ID (index in fd list) to each client
  // (broadcasting example)
  int i;
  for (i = 0; i < 3; i++) {
    char* t = calloc(sizeof(char), 2);
    sprintf(t, "%d", i);
    write(to_client[i], t, 14);
    free(t);
  }

}
