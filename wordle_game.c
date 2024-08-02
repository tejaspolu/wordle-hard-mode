#include "wordle_lib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Once your other functions are working, please revert your main() to its
// initial state, but please feel free to change it, during development. You'll
// want to test out each individual function!
int main(void) {
  char **vocabulary;
  size_t num_words;
  int num_guesses = 0;

  srand(time(NULL));

  // load up the vocabulary and store the number of words in it.
  vocabulary = load_vocabulary("vocabulary.txt", &num_words);
  

  // Randomly select one of the words from the file to be today's SECRET WORD.
  int word_index = rand() % num_words;
  char *secret = vocabulary[word_index];
  //char *secret = "scent";
  // input buffer -- we'll use this to get a guess from the user.
  char guess[80];

  // buffer for scoring each guess.
  char result[6] = {0};
  bool success = false;

  //SET TO TRUE IF YOU WANT TO PLAY HARD MODE
  bool hard_mode_on = true;
  if(hard_mode_on){
    hard_mode mode;
    initialize_hard_mode(&mode);
    //print_hard_mode(mode);
    printf("Playing Hard Mode!\n");
    //printf("secret: %s\n", secret);
    printf("time to guess a 5-letter word! (press ctrl-D to exit)\n");
    while (!success) {
      printf("guess: ");
      if (fgets(guess, 80, stdin) == NULL) {
        break;
      }
      // Whatever the user input, cut it off at 5 characters.
      guess[5] = '\0';
      if (!valid_guess(guess, vocabulary, num_words)) {
        printf("not a valid guess\n");
        continue;
      } else if (!is_valid_hard_guess(&mode, guess)) {
        printf("hey! you can't make that guess, it's inconsistent with what I already told you!\n");
        continue;
      } else {
        num_guesses++;
      }
      success = score_guess(secret, guess, result);
      
      printf("%s\n", guess);
      printf("%s\n", result);
      update_hard_mode(&mode, guess, result);
    }

    if (success) {
      printf("you win, in %d guesses!\n", num_guesses);
    }
    free_vocabulary(vocabulary, num_words);

  } else {
    printf("time to guess a 5-letter word! (press ctrl-D to exit)\n");
    while (!success) {
      printf("guess: ");
      if (fgets(guess, 80, stdin) == NULL) {
        break;
      }
      // Whatever the user input, cut it off at 5 characters.
      guess[5] = '\0';

      if (!valid_guess(guess, vocabulary, num_words)) {
        printf("not a valid guess\n");
        continue;
      } else {
        num_guesses++;
      }
      success = score_guess(secret, guess, result);
      printf("%s\n", guess);
      printf("%s\n", result);
    }
    if (success) {
      printf("you win, in %d guesses!\n", num_guesses);
    }
    free_vocabulary(vocabulary, num_words);
  }
  

  return 0;
}
