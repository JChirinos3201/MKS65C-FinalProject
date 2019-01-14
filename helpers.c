/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

#include "networking.h"
#include "helpers.h"

char** parse_cards(char* raw_cards) {
  char** cards = calloc(sizeof(char*), 500);
  int index = 0;
  char* token;
  while (raw_cards != NULL && index < 500) {
    token = strsep(&raw_cards, "\n");
    // ignores extra \n
    if (strcmp(token, "")) {
      cards[index] = token;
      index++;
    }
  }
  return cards;
}

struct deck* get_white_deck() {
  // make white card struct
  struct deck* white = calloc(sizeof(struct deck), 1);

  // reading and parsing white card csv
  int file = open("cards/finalwhite.csv", O_RDONLY);
  char * raw_cards = calloc(sizeof(char), 10000);
  read(file, raw_cards, 10000);

  // splitting at \n
  char** white_cards = parse_cards(raw_cards);

  // setting deck vals
  white->cards = white_cards;
  white->size = size(white_cards);
  white->card_at = 0;

  return white;
}

struct deck* get_black_deck() {
  // make white card struct
  struct deck* black = calloc(sizeof(struct deck), 1);

  // reading and parsing black card csv
  int file = open("cards/finalblack.csv", O_RDONLY);
  char * raw_cards = calloc(sizeof(char), 10000);
  read(file, raw_cards, 10000);
  char** black_cards = parse_cards(raw_cards);

  // splitting at \n
  black->cards = black_cards;

  // setting deck vals
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
