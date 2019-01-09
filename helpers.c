/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

#include "pipe_networking.h"
#include "helpers.h"

char ** parse_cards(char * raw_cards){
  char ** cards = calloc(500, sizeof(char*));

  int index = 0;

  while (raw_cards != NULL && index < 500){
    cards[index] = strsep(&raw_cards, "\n");
    // printf("yoot:: %s\n", cards[index]);
    index++;
  }
  return cards;
}


struct deck* get_white_deck() {
  // make white card struct
  struct deck* white = calloc(sizeof(struct deck), 1);

  // reading and parsing white card csv
  int file = open("cards/white.csv", O_RDONLY);
  char * raw_cards = calloc(sizeof(char), 10000);
  read(file, raw_cards, 10000);
  printf("RAW WHITE DECK\n%s\nEND\n", raw_cards);
  char** white_cards = parse_cards(raw_cards);
  printf("\n\nSTARTIG CHAR** WHITE\n");
  print_cards(white_cards);
  printf("END\n\n\n");

  // yeeting lots and lots of memory
  free(raw_cards);

  // setting struct values
  white->cards = white_cards;
  // print_cards(white->cards);
  white->size = size(white_cards);
  white->card_at = 0;

  return white;
}

struct deck* get_black_deck() {
  // make white card struct
  struct deck* black = calloc(sizeof(struct deck), 1);

  // reading and parsing black card csv
  int file = open("cards/black.csv", O_RDONLY);
  char * raw_cards = calloc(sizeof(char), 10000);
  read(file, raw_cards, 10000);
  printf("RAW BLACK DECK\n%s\nEND\n", raw_cards);
  char** black_cards = parse_cards(raw_cards);
  printf("\n\nSTARTIG CHAR** BLACK\n");
  print_cards(black_cards);
  printf("END\n\n\n");

  // yeeting lots and lots of memory
  free(raw_cards);

  // setting struct values
  printf("BLACK CARDS BEFORE ASSIGNING TO DECK:\n");
  print_cards(black_cards);
  printf("\n\n\n\n\n");
  black->cards = black_cards;

  printf("\n\nBLACK CARDS AFTER ASSIGNING TO DECK:\n");
  print_cards(black->cards);
  black->size = size(black_cards);
  black->card_at = 0;

  return black;
}

size_t size(char** arr) {
  size_t s = 0;
  while (*arr) {
    s++;
    arr++;
  }
  return s;
}

void swap(struct deck* deck, int i, int j) {
  char* temp = calloc(sizeof(char), 100);
  strcpy(temp, deck->cards[i]);
  deck->cards[i] = deck->cards[j];
  deck->cards[j] = temp;
}

void shuffle(struct deck* deck) {
  int i;
  for (i = deck->size - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    swap(deck, i, j);
  }
}

void print_cards(char** cards) {
  while (*cards) {
    printf("%s\n", *cards);
    cards++;
  }
}
