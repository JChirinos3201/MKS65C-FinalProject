/*
Camilla Cheng, Joan Chirinos
Team Jam In A Can

Cards Against K
*/

#include "pipe_networking.h"
#include "helpers.h"

char ** parse_cards(char * raw_cards){
  char ** args = calloc(500, sizeof(char*));

  int index = 0;

  while (raw_cards != NULL && index < 500){
    args[index] = strsep(&raw_cards, "\n");
    index++;
  }
  return args;
}


char** get_white_cards() {
  int file = open("cards/white.csv", O_RDONLY);
  char * raw_cards = calloc(sizeof(char), 10000);
  read(file, raw_cards, 10000);
  char** cards = parse_cards(raw_cards);

  free(raw_cards);
  return cards;
}

char** get_black_cards() {
  int file = open("cards/black.csv", O_RDONLY);
  char * raw_cards = calloc(sizeof(char), 10000);
  read(file, raw_cards, 10000);
  char** cards = parse_cards(raw_cards);

  free(raw_cards);
  return cards;
}

void shuffle(char ** cards) {

}
