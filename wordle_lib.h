#pragma once

#include <stdbool.h>
#include <stdio.h>



bool score_guess(char *secret, char *guess, char *result);

bool valid_guess(char *guess, char **vocabulary, size_t num_words);

char **load_vocabulary(char *filename, size_t *num_words);

void free_vocabulary(char **vocabulary, size_t num_words);

// HARD MODE FEATURES

typedef struct hard_mode{
    char green[5];
    char yellow[5][6];
    char gray[5][6];
} hard_mode;

void initialize_hard_mode(hard_mode *mode);

void update_hard_mode(hard_mode *mode, char *guess, char *result);

bool is_valid_hard_guess(hard_mode *state, char *guess);

// Idea for the UNUSED macro from Stack Overflow user mtvec. Thanks!
// https://stackoverflow.com/a/3599170
#define UNUSED(x) (void)(x)
