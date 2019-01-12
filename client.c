/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

int MAX_PLAYER_COUNT;

int to_server, from_server;
char** white_cards;
char** submissions;
char** names;
char* black_card;
int* scores;
int card_choice; // rename later if you want dumbo
int player_num;
int czar;

#include "pipe_networking.h"

void display_black_card() {
  printf("Black Card:\n\t%s\n", black_card);
}

void display_white_cards() {
  // display player's white cards
  printf("White Cards:\n");
  for (int i = 0; i < 7; i++){
    printf("\t%d: %s\n", i, white_cards[i]);
  }
  printf("\n");
}

void get_line(char * line) {
  fgets(line, 10, stdin);
  char * p = strchr(line, '\n');
  if (p) *p = 0;
}

void setup() {
  int i;

  // clear terminal
  printf("\033[2J\n");
  printf("\033[2J\n");

  // memory allocation
  submissions = calloc(sizeof(char*), MAX_PLAYER_COUNT);
  white_cards = calloc(sizeof(char*), 7);
  black_card = calloc(sizeof(char), 200);
  names = calloc(sizeof(char*), MAX_PLAYER_COUNT);
  scores = calloc(sizeof(int), MAX_PLAYER_COUNT);

  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    scores[i] = 0;
  }

  from_server = client_handshake(&to_server);

  // gets player number
  char* response = calloc(sizeof(char), 2);
  read(from_server, response, 2);
  // printf("You are player #%s\n", response);
  player_num = atoi(response);

  // sets MAX_PLAYER_COUNT
  char* mpc_string = calloc(sizeof(char), 2);
  read(from_server, mpc_string, 2);
  MAX_PLAYER_COUNT = atoi(mpc_string);
  free(mpc_string);

  // gets white cards
  for (i = 0; i < 7; i++){
    char* card = calloc(sizeof(char), 200);
    read(from_server, card, 200);
    white_cards[i] = card;
  }

  free(response);

  // send name
  printf("What's your name?\nName: ");
  char* name = calloc(sizeof(char), 50);
  fgets(name, 50, stdin);
  char * p = strchr(name, '\n');
  if (p) *p = 0;
  write(to_server, name, 50);
  free(name);

  // gets player names
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    char* name = calloc(sizeof(char), 50);
    read(from_server, name, 50);
    names[i] = name;
  }

}


/********************
  CZAR FUNCTIONS
********************/

void get_czar() {
  // printf("start get_czar\n");
  char* response = calloc(sizeof(char), 2);
  read(from_server, response, 2);
  // printf("read %s\n", response);
  czar = atoi(response);
}

void get_player_submissions() {
  // printf("start get_player_submissions\n");
  // might have a lot of memory leakage here...
  // clear previous submissions
  printf("YOU ARE THE CZAR\n");
  memset(submissions, 0, MAX_PLAYER_COUNT);
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    char* t = calloc(sizeof(char), 200);
    read(from_server, t, 200);
    // printf("read %s\n", t);
    submissions[i] = t;
    // printf("player #%d submitted |%s|\n", i, submissions[i]);
  }
}

void select_winner() {
  // printf("start select_winner\n");
  printf("\033[2J\n");
  int i;
  int winner;
  display_black_card();
  // display player submissions
  printf("\nPlayer Submissions\n");
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    if (i != player_num) {
      printf("\t%d. %s\n", i, submissions[i]);
    }
  }

  // select winner
  printf("\nWinner: ");
  char* winner_string = calloc(sizeof(char), 10);
  get_line(winner_string);
  // printf("read |%s| from user\n", winner_string);

  // send winner index to server
  write(to_server, winner_string, 2);
  // printf("wrote |%s| to server\n", winner_string);

}

/********************
  NON-CZAR FUNCTIONS
********************/

void get_black_card() {
  // printf("start get_black_card\n");
  read(from_server, black_card, 200);
  // printf("read |%s|\n", black_card);
}

void submit_white_card() {
  // printf("start submit_white_card\n");
  printf("Your selection: ");
  char * line = calloc(sizeof(char), 10);
  get_line(line);
  card_choice = atoi(line);
  // printf("sending card #%d: %s\n", card_choice, white_cards[card_choice]);
  write(to_server, white_cards[card_choice], 200);
  // printf("card sent\n");
  printf("Waiting for winner selection...\n");
}

void endgame_check() {
  // printf("start endgame_check\n");
  // reading winning index, or -1 if nobody won
  char* winner = calloc(sizeof(char), 10);
  read(from_server, winner, 10);
  // printf("read |%s|\n", winner);
  int winning_index = atoi(winner);
  free(winner);

  if (winning_index != -1) {
    // display who won and whatnot
    exit(EXIT_SUCCESS);
  }
  // else it must be -1, which means nobody won, so just keep going with the rest of the code
}

void get_white_card() {
  // printf("start get_white_card\n");
  read(from_server, white_cards[card_choice], 200);
  // printf("read |%s|\n", white_cards[card_choice]);
}

void print_screen() {

  // clear screen
  printf("\033[2J\n");

  // generic loop counter
  int i;

  // print scores
  printf("Scores:\n");
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    printf("\t%s: %s\n", names[i], "please fix the score thing <3");
  }

  // print black card
  display_black_card();

  printf("\n");

  // print white card
  display_white_cards();

  // then, you would print some request, such as submit_white_card()
}


void play() {
  while (1) {
    get_czar();
    get_black_card();
    print_screen();
    if (player_num == czar) {
      get_player_submissions();
      select_winner();
    }
    else {
      submit_white_card();
      get_white_card();
    }
    endgame_check();
  }
}

int main() {
  setup();
  play();
}
