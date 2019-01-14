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
int round_winner;

#include "pipe_networking.h"

void display_black_card() {
  printf("The black card for this round is:\n\t%s\n", black_card);
}

void display_white_cards() {
  // display player's white cards
  printf("White Cards:\n");
  for (int i = 0; i < 7; i++){
    printf("\t%d: %s\n", i, white_cards[i]);
  }
  printf("\n");
}

void get_player_submissions() {
  // printf("start get_player_submissions\n");
  // might have a lot of memory leakage here...
  // clear previous submissions
  memset(submissions, 0, MAX_PLAYER_COUNT - 1);
  int i;
  for (i = 0; i < MAX_PLAYER_COUNT - 1; i++) {
    char* t = calloc(sizeof(char), 200);
    read(from_server, t, 200);
    // printf("read %s\n", t);
    submissions[i] = t;
    // printf("player #%d submitted |%s|\n", i, submissions[i]);
  }
}

void display_submissions() {
  printf("\033[2J\n");
  int i;
  display_black_card();
  // display player submissions
  printf("\nPlayer Submissions\n");
  for (i = 0; i < MAX_PLAYER_COUNT - 1; i++) {
    printf("\t%d. %s\n", i, submissions[i]);
  }
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

  // print initial message
  printf("Waiting for players...\n");

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

  printf("\033[2J\n");

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

void select_winner() {
  // printf("start select_winner\n");

  // select winner
  printf("\nWinner: ");
  char* winner_string = calloc(sizeof(char), 10);
  get_line(winner_string);
  // printf("read |%s| from user\n", winner_string);

  // send winner index to server
  write(to_server, winner_string, 2);
  // printf("wrote |%s| to server\n", winner_string);

}

void czar_status(){
  printf("--- YOU ARE THE CZAR : Waiting For Players... ---\n");
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
}

void get_white_card() {
  // printf("start get_white_card\n");
  read(from_server, white_cards[card_choice], 200);
  // printf("read |%s|\n", white_cards[card_choice]);
}

void player_status(){
  printf("\nWaiting for winner selection...\n");
}

/********************
  GAME FUNCTIONS
********************/

void get_round_winner(){
  // get winner
  char* round_winner_string = calloc(sizeof(char), 2);
  read(from_server, round_winner_string, 2);
  round_winner = atoi(round_winner_string);

  printf("\n%s wins this round!\n\n", names[round_winner]);
  // increase score
  scores[round_winner]++;
  free(round_winner_string);
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
    printf("\033[2J");

    // print scores
    printf("Final Scores:\n");
    for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
      printf("\t%s: %d Brownie Point", names[i], scores[i]);
      if (scores[i] != 1) {
        printf("s");
      }
      printf("\n");
    }
    printf("..................................................\n\n");

    printf("\t---  %s IS THE WINNER!!! ---\n\n", names[winning_index]);

    // fireworks pls dont take these out i spent time putting this here
    // ascii art by Joan Stark (not this Joan) on www.asciiart.eu
    printf("                                   .''.       \n");
    printf("       .''.      .        *''*    :_\\/_:     . \n");
    printf("      :_\\/_:   _\\(/_  .:.*_\\/_*   : /\\ :  .'.:.'.\n");
    printf("  .''.: /\\ :   ./)\\   ':'* /\\ * :  '..'.  -=:o:=-\n");
    printf(" :_\\/_:'.:::.    ' *''*    * '.\\'/.' _\\(/_'.':'.'\n");
    printf(" : /\\ : :::::     *_\\/_*     -= o =-  /)\\    '  *\n");
    printf("  '..'  ':::'     * /\\ *     .'/.\\'.   '\n");
    printf("      *            *..*         :\n");
    printf("      *\n\n");

    exit(EXIT_SUCCESS);
  }
  // else it must be -1, which means nobody won, so just keep going with the rest of the code
}

void print_screen() {

  // clear screen
  printf("\033[2J");

  // generic loop counter
  int i;

  // print scores
  printf("Scores:\n");
  for (i = 0; i < MAX_PLAYER_COUNT; i++) {
    printf("\t%s: %d Brownie Point", names[i], scores[i]);
    if (scores[i] != 1) {
      printf("s");
    }
    if (i == czar){
      printf("\t---> CARD CZAR");
    }
    printf("\n");
  }
  printf("\n");

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
      czar_status();
      get_player_submissions();
      display_submissions();
      select_winner();
    }
    else {
      submit_white_card();
      get_white_card();
      get_player_submissions();
      display_submissions();
      player_status();
    }
    get_round_winner();
    endgame_check();
  }
}

int main() {
  setup();
  play();
}
