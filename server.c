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
struct deck* black_deck;
struct deck* white_deck;
int* to_client;
int* from_client;
int turn_counter;
int czar;
int* scores;
char** names;

void setup() {
  // seeding rand
  srand(time(NULL));

  // set ctrl-c signal
  signal(SIGINT, sighandler);

  // decks of black and white cards
  black_deck = get_black_deck();
  white_deck = get_white_deck();

  // initializes turn_counter
  turn_counter = 0;

  // shuffle decks
  shuffle(black_deck);
  shuffle(white_deck);

  // array of file descriptors for pipes to/from clients
  to_client = calloc(sizeof(int), MAX_PLAYER_COUNT);
  from_client = calloc(sizeof(int), MAX_PLAYER_COUNT);

  // array of player names
  names = calloc(sizeof(char*), MAX_PLAYER_COUNT);

  // array of player scores
  scores = calloc(sizeof(int), MAX_PLAYER_COUNT);

  printf("waiting for clients to connect...\n");
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

  // sends client's player #
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

  // print_cards(black_deck->cards);
  // printf("TEST BLACK: %s\n", black_deck->cards[3]);
  // printf("TEST WHITE: %s\n", white_deck->cards[3]);

}

void broadcast_czar() {
  czar = turn_counter % MAX_PLAYER_COUNT;
  for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
    char* czar_num = calloc(sizeof(char), 2);
    sprintf(czar_num, "%d", czar);
    write(to_client[i], czar_num, 2);
    free(czar_num);
  }
}

void broadcast_black_card() {
  for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
    write(to_client[i], black_deck->cards[black_deck->card_at], 200);
  }
}

void get_white_cards() {
  // reads the cards selected by players who are NOT the czar
  char ** cards_selected = calloc(sizeof(char*), MAX_PLAYER_COUNT - 1);
  int i;
  int n = 0;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    if (i != czar) {
      char* card = calloc(sizeof(char), 200);
      read(from_client[i], card, 200);
      cards_selected[n] = card;
      n++;
    }
  }

  // writes cards to the card czar
  for (i = 0; i < MAX_PLAYER_COUNT - 1; i++){
    write(to_client[czar], cards_selected[i], 200);
  }
}

void select_winner() {

}

int endgame_check() {
  return 0;
}

void distribute_white_cards() {
  printf("distributing white cards...\n");
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    write(to_client[i], white_deck->cards[white_deck->card_at], 200);
    white_deck->card_at++;
  }
  black_deck->card_at++;
  turn_counter++;
}

void play() {
  for(int n = 0; n < 2; n++){ // for testing purposes, runs 2 turns
  // while (1) {
    broadcast_czar();
    broadcast_black_card();
    get_white_cards();
    select_winner();
    // if (endgame_check()) {
    //   break;
    // }
    distribute_white_cards();
  }
}

int main() {
  setup();
  play();
}
