/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

/*
MVP

No settings; predefined values
*/

#include "networking.h"
#include "helpers.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\n");
    exit(EXIT_SUCCESS);
  }
}

int MAX_PLAYER_COUNT = 3;
int SCORE_CAP = 2;

// defining useful vars
struct deck* black_deck;
struct deck* white_deck;
char** names;
char** cards_selected;
int* clients;
int* scores;
int* submission_indexes;
int turn_counter;
int czar;
int round_winner;

void setup() {
  int i;
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
  clients = calloc(sizeof(int), MAX_PLAYER_COUNT);

  // array of player names
  names = calloc(sizeof(char*), MAX_PLAYER_COUNT);

  // array of player scores
  scores = calloc(sizeof(int), MAX_PLAYER_COUNT);
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    scores[i] = 0;
  }

  // array of submission indexes
  submission_indexes = calloc(sizeof(int), MAX_PLAYER_COUNT - 1);

  // array of cards selected per round
  cards_selected = calloc(sizeof(char*), MAX_PLAYER_COUNT);

  // basically WKP?
  int listen_socket = server_setup();

  printf("waiting for clients to connect...\n");
  // populate client socket list
  int player_count = 0;
  while (player_count < MAX_PLAYER_COUNT) {
    clients[player_count] = server_connect(listen_socket);
    // clients[player_count] = server_handshake(clients + player_count);
    player_count++;
  }
  close(listen_socket);
  printf("all clients connected\n");

  // broadcasting ID (index in fd list) to each client

  // sends client's player #
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    char* t = calloc(sizeof(char), 2);
    sprintf(t, "%d", i);
    write(clients[i], t, 2);
    free(t);
  }

  // sends clients MAX_PLAYER_COUNT
  char* m = calloc(sizeof(char), 2);
  sprintf(m, "%d", MAX_PLAYER_COUNT);
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    write(clients[i], m, 2);
  }

  // sending 7 cards to each client
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    for (int c = 0; c < 7; c++) {
      write(clients[i], white_deck->cards[white_deck->card_at], 200);
      white_deck->card_at++;
    }
  }

  // get player names
  names = calloc(sizeof(char*), MAX_PLAYER_COUNT);
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    char* name = calloc(sizeof(char), 50);
    read(clients[i], name, 50);
    names[i] = name;
  }

  // send players list of names
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    int j;
    for (j = 0; j < MAX_PLAYER_COUNT; j++) {
      write(clients[i], names[j], 50);
    }
  }

}

void broadcast_czar() {
  czar = turn_counter % MAX_PLAYER_COUNT;
  char* czar_num = calloc(sizeof(char), 2);
  sprintf(czar_num, "%d", czar);
  for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
    write(clients[i], czar_num, 2);
  }
  free(czar_num);
}

void broadcast_black_card() {
  for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
    write(clients[i], black_deck->cards[black_deck->card_at], 200);
  }
  black_deck->card_at++;
}

void get_white_cards() {
  // clears memory of selected cards
  memset(cards_selected, 0, MAX_PLAYER_COUNT);

  // gets cards from players. czar index is left at 0
  int index = 0;
  for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
    if (i != czar) {
      char* card = calloc(sizeof(char), 200);
      read(clients[i], card, 200);
      cards_selected[i] = card;
      submission_indexes[index] = i;
      index++;
    }
    else {
      cards_selected[i] = "The Czar";
    }
  }
}

// void randomize_submissions() {
//   int i;
//   for (i = MAX_PLAYER_COUNT - 2; i > 0; i--) {
//     int j = rand() % (i + 1);
//     swap(submission_indexes, i, j);
//   }
// }

void randomize_submissions() {
  int n = MAX_PLAYER_COUNT - 1;
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = submission_indexes[j];
      submission_indexes[j] = submission_indexes[i];
      submission_indexes[i] = t;
    }
  }
}

void send_player_submissions() {
  printf("sending card submissions to players\n");
  for (int i = 0; i < MAX_PLAYER_COUNT; i++){
    for (int index = 0; index < MAX_PLAYER_COUNT - 1; index++){
    // printf("writing %s\n", cards_selected[i]);
      write(clients[i], cards_selected[submission_indexes[index]], 200);
    }
  }
}

void get_round_winner() {
  // get winner
  char* round_winner_string = calloc(sizeof(char), 2);
  read(clients[czar], round_winner_string, 2);
  round_winner = submission_indexes[atoi(round_winner_string)];

  // increase score
  scores[round_winner]++;
  free(round_winner_string);
}

void send_round_winner(){
  char* round_winner_string = calloc(sizeof(char), 2);
  sprintf(round_winner_string, "%d", round_winner);
  for (int i = 0; i < MAX_PLAYER_COUNT; i++){
    write(clients[i], round_winner_string, 2);
  }
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
    write(clients[i], t, 10);
  }

  // freeing memory
  free(winner);

  if (winning_index != -1) {
    // display who won and whatnot
    printf("exiting game...\n");
    exit(EXIT_SUCCESS);
  }
}

void distribute_white_cards() {
  printf("distributing white cards...\n");
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    if (i != czar) {
      write(clients[i], white_deck->cards[white_deck->card_at], 200);
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
    randomize_submissions();
    send_player_submissions();
    get_round_winner();
    send_round_winner();

    endgame_check();
    int i;
    for (i = 0; i < MAX_PLAYER_COUNT; i++) {
      printf("%s: %d\n", names[i], scores[i]);
    }
  }
}

int main() {
  setup();
  play();
}
