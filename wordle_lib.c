#include "wordle_lib.h"
#include <string.h>
#include <stdlib.h>

// Most of the code that you're going to have to implement goes in here.
// Feel free to add more helper functions if necessary.

// HARD MODE FEATURES

void initialize_hard_mode(hard_mode *mode){
  //setting all the values to null
  for(int i = 0; i < 5; i++){
    mode->green[i] = '\0';
    for(int j = 0; j < 6; j++){
      mode->yellow[i][j] = '\0';
      mode->gray[i][j] = '\0';
    }
  }
}

void update_hard_mode(hard_mode *mode, char *guess, char *result){
  for(int i = 0; i < 5; i++){
    if (result[i] == 'g'){
      //setting the ith index of green to the guess
      mode->green[i] = guess[i];
    } else if (result[i] == 'y'){
      //setting the first non null character at the ith index to the guess
      mode->yellow[i][strcspn(mode->yellow[i], "\0")] = guess[i];
    } else if (result[i] == 'x'){
      //checking if character is already in yellow
      bool char_in_yellow = false;
      for (int j = 0; j < 5; j++) {
        if (strchr(mode->yellow[j], guess[i])) {
            char_in_yellow = true;
        }
      }
      if(!char_in_yellow){
        //setting the first non null character at the ith index to the guess
        mode->gray[i][strcspn(mode->gray[i], "\0")] = guess[i];
      }
    }
  }
}


bool is_valid_hard_guess(hard_mode *mode, char *guess){
  //bool array of used yellow characters
  bool used_yellow_chars[5] = {false};
  for(int i = 0; i < 5; i++){
    //if mode->green[i] isnt null and if it isnt equal to the guess at i
    if (mode->green[i] != '\0' && mode->green[i] != guess[i]){
      return false;
    }
    for(int j = 0; j < 5; j++){
      if (strchr(mode->gray[j], guess[i])){
        return false; 
      }
    }
  }

  for(int i = 0; i < 5; i++){
    if (mode->yellow[i][0] != '\0'){
      for(int j = 0; mode->yellow[i][j] != '\0'; j++){
        if (strchr(guess, mode->yellow[i][j])){
          for(int k = 0; k < 5; k++){
            if (guess[k] == mode->yellow[i][j] && k != i){
                used_yellow_chars[i] = true;
                break;
            }
          }
        }
      }
    }
  }

  for(int i = 0; i < 5; i++){
    if (mode->yellow[i][0] != '\0' && !used_yellow_chars[i]){
      return false;
    }
  }
  return true;
}

// Returns true if the guess is an exact match with the secret word, but
// more importantly, fills in the result with the following:
// - 'x' goes in a slot if the corresponding letter in the guess does not appear
//   anywhere in the secret word
// - 'y' goes in a slot if the corresponding letter in the guess appears in the
//   secret word, but not in the corresponding slot
// - 'g' goes in a slot if the corresponding letter in the guess appears in the
//   corresponding slot in the secret word.
//   You can assume that result points at enough memory for a string of length
//   5. (ie, at least 6 bytes long)
bool score_guess(char *secret, char *guess, char *result){
  bool secret_match[5] = {false};
  //misses
  for(int i = 0; i < 5; i++){
    result[i] = 'x';
  }
  //hits
  for(int i = 0; i < 5; i++){
    if (guess[i] == secret[i]){
      result[i] = 'g';
      secret_match[i] = true;
    }
  }
  //semi hits
  for(int i = 0; i < 5; i++){
    if (result[i] == 'x'){
      for(int j = 0; j < 5; j++){
        if (secret[j] == guess[i] && !secret_match[j] && result[i] != 'g'){
          result[i] = 'y';
          secret_match[j] = true;
          break;
        }
      }
    }
  }

  result[5] = '\0';
  for(int i = 0; i < 5; i++){
    if(result[i] != 'g'){
      return false;
    }
  }

  return true;
}

// Returns true if the specified guess is one of the strings in the vocabulary,
// and false otherwise. num_words is the length of the vocabulary.
// A simple linear scan over the strings in vocabulary is fine for our purposes,
// but consider: could you do this search more quickly?
bool valid_guess(char *guess, char **vocabulary, size_t num_words){
  // TODO(you): finish this function
  for(size_t i = 0; i < num_words; i++){
    if(strcmp(guess, vocabulary[i]) == 0){
      return true;
    }
  }
  return false;
}

// Returns an array of strings (so, char **), where each string contains a word
// from the specified file. The file is assumed to contain 5-letter words, one
// per line.
// Also, this function sets the value pointed at by *num_words to be the number
// of words read.
// This will need to allocate enough memory to hold all of the char* pointers --
// so you will keep track of the size of your char** array and then use realloc
// to make the array larger over time, so that you have enough space for the
// dynamically-growing array of char *.
// Use fopen to open the input file for reading,
// strdup (or strndup) to make copies of each word read from that file, and
// fclose to close the file when you are done reading from it.
// Each element of the array should be a single five-letter word,
// null-terminated.
char **load_vocabulary(char *filename, size_t *num_words){
  FILE *file = fopen(filename, "r");
  char **out = NULL;
  char buffer[6];
  size_t counter = 0;

  while(fgets(buffer, 6, file)){
    buffer[strcspn(buffer, "\n")] = '\0';
    if(strlen(buffer) == 0){
      continue;
    }
    if(out == NULL){
      out = (char **)malloc(sizeof(char *));
    } else {
      out = realloc(out, sizeof(char *) * (counter + 1));
    }
    out[counter] = strdup(buffer);
    counter++;
  }

  *num_words = counter;
  fclose(file);
  return out;
}
// char **load_vocabulary(char *filename, size_t *num_words){
//     FILE *file = fopen(filename, "r");
//     if (!file){
//         *num_words = 0;
//         return NULL;  // Handle the case where the file couldn't be opened.
//     }

//     char **out = NULL;
//     char buffer[6];  // Enough space for 5 letters plus a null terminator.
//     size_t counter = 0;

//     while (fgets(buffer, sizeof(buffer), file)){
//         buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character.
//         if (strlen(buffer) == 0) continue;  // Skip empty lines if any.

//         char **new_out = realloc(out, sizeof(char *) * (counter + 1));
//         if (!new_out){
//             // Cleanup already allocated memory if realloc fails.
//             for(size_t i = 0; i < counter; i++){
//                 free(out[i]);
//             }
//             free(out);
//             fclose(file);
//             *num_words = 0;
//             return NULL;
//         }
//         out = new_out;
//         out[counter] = strdup(buffer);
//         if (!out[counter]){
//             // If strdup fails, cleanup and exit.
//             for(size_t i = 0; i < counter; i++){
//                 free(out[i]);
//             }
//             free(out);
//             fclose(file);
//             *num_words = 0;
//             return NULL;
//         }
//         counter++;
//     }

//     fclose(file);
//     *num_words = counter;
//     return out;
// }

// Free each of the strings in the vocabulary, as well as the pointer vocabulary
// itself (which points to an array of char *).
void free_vocabulary(char **vocabulary, size_t num_words){
  // TODO(you): finish this function
  for(size_t i = 0; i < num_words; i++){
    free(vocabulary[i]);
  }
  free(vocabulary);
}
