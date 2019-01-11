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
int SCORE_CAP = 2;

// defining useful vars
struct deck* black_deck;
struct deck* white_deck;
int* to_client;
int* from_client;
int turn_counter;
int czar;
int* scores;
char** names;
char** cards_selected;

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

  // array of cards selected per round
  cards_selected = calloc(sizeof(char*), MAX_PLAYER_COUNT);

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

  // sends clients MAX_PLAYER_COUNT
  char* m = calloc(sizeof(char), 2);
  sprintf(m, "%d", MAX_PLAYER_COUNT);
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    write(to_client[i], m, 2);
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
  black_deck->card_at++;
}

void get_white_cards() {
  // clears memory of selected cards
  memset(cards_selected, 0, MAX_PLAYER_COUNT);

  // gets cards from players. czar index is left at 0
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    if (i != czar) {
      char* card = calloc(sizeof(char), 200);
      read(from_client[i], card, 200);
      cards_selected[i] = card;
    }
    else {
      cards_selected[i] = "i am the czar";
    }
  }
}

void send_player_submissions() {
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++){
    printf("writing %s\n", cards_selected[i]);
    write(to_client[czar], cards_selected[i], 200);
  }
}

void get_round_winner() {
  // get winner
  char* round_winner_string = calloc(sizeof(char), 2);
  read(from_client[czar], round_winner_string, 2);
  int round_winner = atoi(round_winner_string);

  // increase score
  scores[round_winner]++;
}

void endgame_check() {
  // find a winner, if exists
  int winning_index = -1;
  int index_at;
  for (index_at = 0; index_at < MAX_PLAYER_COUNT; index_at++) {
    if (scores[index_at] == SCORE_CAP) {
      winning_index = index_at;
    }
  }

  // broadcast winner index, or -1 if no winner, to clients
  char* winner = calloc(sizeof(char), 10);
  sprintf(winner, "%d", winning_index);
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    char* t = calloc(sizeof(char), 10);
    strcpy(t, winner);
    write(to_client[i], t, 10);
  }

  // freeing memory
  free(winner);

  if (winning_index != -1) {
    // display who won and whatnot
    exit(EXIT_SUCCESS);
  }
}

void distribute_white_cards() {
  printf("distributing white cards...\n");
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    if (i != czar) {
      write(to_client[i], white_deck->cards[white_deck->card_at], 200);
      white_deck->card_at++;
    }
  }
  turn_counter++;
}

void play() {
  // for(int n = 0; n < 2; n++){ // for testing purposes, runs 2 turns
  while (1) {
    // for non-czars
    broadcast_czar();
    broadcast_black_card();
    get_white_cards();
    distribute_white_cards();

    // for czars
    send_player_submissions();
    get_round_winner();

    endgame_check();
  }
}

int main() {
  setup();
  play();
}
