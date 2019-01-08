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
  // seeding rand
  srand(time(NULL));

  // set ctrl-c signal
  signal(SIGINT, sighandler);

  // decks of black and white cards
  struct deck* black_deck = get_black_deck();
  struct deck* white_deck = get_white_deck();

  // shuffle decks
  // shuffle(black_deck);
  // shuffle(white_deck);

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
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    char* t = calloc(sizeof(char), 2);
    sprintf(t, "%d", i);
    write(to_client[i], t, 14);
    free(t);
  }

  // sending 7 cards to each client
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    for (int c = 0; c < 7; c++) {
      int at = white_deck->card_at;
      printf("writing %s\n", white_deck->cards[at]);
      write(to_client[i], white_deck->cards[at], 200);
      white_deck->card_at++;
      char* xD = calloc(10, 100);
      read(from_client[i], xD, 100);
      free(xD);
    }
  }



}
