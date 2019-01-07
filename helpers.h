#ifndef HELPERS_H
#define HELPERS_H

char ** parse_cards(char *);

struct deck* get_white_deck();

struct deck* get_black_deck();

size_t size(char**);

void swap(struct deck*, int, int);

void shuffle(struct deck*);

void print_cards(char**);

#endif
