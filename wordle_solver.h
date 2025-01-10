#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

// Constants
extern const int WORD_ARR_LENGTH;
extern const int WORD_LENGTH;
extern const int MUST_NOT_LENGTH;
extern const int DICT_LENGTH;

/*
* This structure is used to store information we have obtained
* from past guesses for each specific position in a 5-letter word
* The "must_be" field is set to a letter if we have inferred the
* letter for this position
* The must_not_be array contains a C string with letters that this 
* position cannot have. By always keeping this a C string (null terminated)
* we will be able to search it easily.
*/
struct position_restrictions {
    char must_be;
    char must_not_be[26];
};

/*
* This structure contains an array for the position_restrictions for the
* five positions in a valid wordle guess.
* It also contains a C string that contains letters which we know a 
* correct guess must contain. By always keeping this a C string (null 
* terminated) we will be able to search it easily.
*/
struct word_analysis {
    struct position_restrictions restrictions[5];
    char must_contain[6];
};

// contains_char(c, word) returns true if word contains c else
//   returns false
// requires: word is a valid C string (not asserted)
bool contains_char(char c, char word[]);

// check_for_ones(arr, len) returns true if arr contains only 1's 
//  and false otherwise
bool check_for_ones(int arr[], int len);

//  auto_select(guess,dict,length,analysis) will automatically select the FIRST 
//    WORD from "dict" that SATISFIES the restrictions specified in "analysis" and
//    place that word in "guess"
//  A word SATISFIES the restrictions if and only if:
//    - each letter in the word satisfies the corresponding must_be and must_not_be
//      restrictions specified within the corresponding letter_restrictions inside
//      "analysis", i.e.,
//      - the i-th character must_be the character in analysis->restrictions[i].must_be
//        if it is not null
//      - the i-th character must_not_be a character found in the string 
//        analysis->restrictions[i].must_not_be
//   - it contains all the letters currently in the string analysis->must_contain 
// effects: updates guess
// requires: guess has enough space to store a 5-letter C string (not asserted)
//           dict is an array of 5-letter C Strings of length "length" (not asserted)
//           analysis is not null
void auto_select(char guess[], char dict[][6], const int length, struct word_analysis *analysis);

// add_restriction(arr,length,let) adds the character let to the C string arr
//   if let is not already within arr.
// effects: arr may be updated
// requires: arr is a valid C Style string of length "length" (not asserted)
//           arr was initialized with every element containing '\0'
void add_restriction(char arr[], const int length, char let);

// analyze_result(guess,result,analysis) goes through each character in result and
//   adds the appropriate restriction by updating analysis (use helper add_restriction)
//   - If the i-th character in result is a G, the letter is in the correct spot so 
//     the must_be character in the corresponding letter_restriction is updated
//   - If the i-th character is an X, the letter does not occur in the wordle, so 
//     the must_not_be restriction for all letter_restrictions is updated.
//   - If the i-th character is a Y, the letter does occur but is miss-placed, so the
//     must_contain list AND the i-th must_not_be lists are updated
// requires: guess is a valid 5-letter C strings (not asserted)
//           result is a valid 5-letter C string comprised of only G, Y and X (not asserted)
//           analysis is not null
void analyze_result(char guess[], char result[], struct word_analysis *analysis);

// load_words(filename, words) copies the strings from filename 
//  into the string array "words" 
// effects: modifies words
void load_words(const char *filename, char dict[][6]);

// pick_word(word, dict, len) generates a random string from dict and stores 
//  it in word
// effects: modifies word
// requires: len is the length of dict
void pick_word(char word[], char dict[][6], int len);


// process_guess(guess, result, wordle) compares the guessed word "guess" with
//   the current "wordle" and sets "result" based on the following rules:
//   - Set the i-th character to G if the guessed letter is in the correct spot
//   - Set the i-th character to Y if the guessed letter is in the word, but is 
//   not in the correct spot
//   - Set the i-th character to X if the guessed letter is not in the word
// effects: result is updated to contain a C string comprised of only G, Y and X
// requires: guess and wordle are valid 5-letter C strings (not asserted)
//           result has enough space to store a 5-letter C string (not asserted)
void process_guess(char guess[], char wordle[], char output[]);

#endif
