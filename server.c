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
    remove(CLIENT_TO_SERVER);
    printf("\n");
    exit(EXIT_SUCCESS);
  }
}

int MAX_PLAYER_COUNT = 3;
int SCORE_CAP = 10;

// defining useful vars
struct deck* black_deck, * white_deck;
int* to_client, * from_client, * scores;
char** names;

void setup() {
  // seeding rand
  srand(time(NULL));

  // set ctrl-c signal
  signal(SIGINT, sighandler);

  // decks of black and white cards
  black_deck = get_black_deck();
  white_deck = get_white_deck();

  // shuffle decks
  // shuffle(black_deck);
  // shuffle(white_deck);

  // array of file descriptors for pipes to/from clients
  to_client = calloc(sizeof(int), MAX_PLAYER_COUNT);
  from_client = calloc(sizeof(int), MAX_PLAYER_COUNT);

  // array of player names
  names = calloc(sizeof(char*), MAX_PLAYER_COUNT);

  // array of player scores
  scores = calloc(sizeof(int), MAX_PLAYER_COUNT);


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
    write(to_client[i], t, 2);
    free(t);
  }

  // sending 7 cards to each client
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    for (int c = 0; c < 7; c++) {
      write(to_client[i], white_deck->cards[white_deck->card_at], 200);
      white_deck->card_at++;
    }
  }

}

void broadcast_black_card() {
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    write(to_client[i], black_deck->cards[black_deck->card_at], 200);
    printf("writing black: %s",  black_deck->cards[black_deck->card_at]);
  }
}

void get_white_cards() {

}

void select_winner() {

}

int endgame_check() {

}

void distribute_white_cards() {
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    write(to_client[i], white_deck->cards[white_deck->card_at], 200);
    white_deck->card_at++;
  }
  black_deck->card_at++;
}

void play() {
  // while (1) {
    broadcast_black_card();
    get_white_cards();
    select_winner();
    // if (endgame_check()) {
    //   break;
    // }
    distribute_white_cards();
  // }
}

int main() {
  setup();
  play();
}
