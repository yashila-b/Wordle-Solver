#include "wordle_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

// Constants
const int WORD_ARR_LENGTH = 6;
const int WORD_LENGTH = 5;
const int MUST_NOT_LENGTH = 26;
const int DICT_LENGTH = 10230;

// contains_char(c, word) returns true if word contains c else
//   returns false
// requires: word is a valid C string (not asserted)
bool contains_char(char c, char word[]) {
	const int len = strlen(word);
	for (int i = 0; i < len; ++i) {
		if (c == word[i]) {
			return true;
		}
	}
	return false;
}

// check_for_ones(arr, len) returns true if arr contains only 1's 
//  and false otherwise
bool check_for_ones(int arr[], int len) {
	assert(arr);

	for (int i = 0; i < len; ++i) {
		if (arr[i] != 1) {
			return false;
		}
	}
	return true;
}

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
void auto_select(char guess[], 
    char dict[][6], const int length, struct word_analysis *analysis) {
		assert(analysis);
		
		int sure_len = strlen(analysis->must_contain);
		for (int i = 0 ; i < length; ++i) {
			bool valid = true;
            int *sure = calloc(sure_len, sizeof(int));
			for (int k = 0; k < WORD_LENGTH; ++k) {
				valid = !contains_char(dict[i][k], analysis->restrictions[k].must_not_be);
				if (analysis->restrictions[k].must_be) {
					valid = valid && (analysis->restrictions[k].must_be == dict[i][k]);
				}
				if (!valid) {
					break;
				}
				guess[k] = dict[i][k];
			}
			for (int k = 0; k < sure_len; ++k) {
				if (contains_char(analysis->must_contain[k], guess)) {
					sure[k] = 1;
				}
			}
			if (valid && check_for_ones(sure, sure_len)) {
                free(sure);
				return;
			}
            free(sure);
		}
}

// add_restriction(arr,length,let) adds the character let to the C string arr
//   if let is not already within arr.
// effects: arr may be updated
// requires: arr is a valid C Style string of length "length" (not asserted)
//           arr was initialized with every element containing '\0'
void add_restriction(char arr[],const int length, char let){
	for (int i = 0; i < length; ++i) { 
		if (arr[i] == let) {
			return; 
		}
	}
	for (int i = 0; i < length - 1; ++i){ 
		if (arr[i] == '\0') { 
			arr[i] = let;
			break;
		}
	}
}

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
void analyze_result(char guess[], char result[], struct word_analysis *analysis) {
	assert(analysis);

	for (int i = 0; i < WORD_LENGTH; ++i) {
		if (result[i] == 'G') {
			analysis->restrictions[i].must_be = guess[i];
		} else if (result[i] == 'X') {
			for (int k = 0; k < WORD_LENGTH; ++k) {
				add_restriction(analysis->restrictions[k].must_not_be,
				 MUST_NOT_LENGTH, guess[i]);
			}
		} else {
			add_restriction(analysis->must_contain, WORD_ARR_LENGTH, guess[i]);
			add_restriction(analysis->restrictions[i].must_not_be, MUST_NOT_LENGTH, guess[i]);
		}
	}
}

// load_words(filename, words) copies the strings from filename 
//  into the string array "words" 
// effects: modifies words
void load_words(const char *filename, char dict[][6]) {
    assert(filename);
    assert(dict);

    FILE *file = fopen(filename, "r");
    
    int count = 0;
    while (fscanf(file, "%5s", dict[count]) == 1) {
        count++;
    }
    fclose(file);
}

// pick_word(word, dict, len) generates a random string from dict and stores 
//  it in word
// effects: modifies word
// requires: len is the length of dict
void pick_word(char word[], char dict[][6], int len) {
    assert(dict);
    assert(word);

    srand(time(NULL));
    int random = rand() % len;
    strcpy(word, dict[random]);
}

// process_guess(guess, result, wordle) compares the guessed word "guess" with
//   the current "wordle" and sets "result" based on the following rules:
//   - Set the i-th character to G if the guessed letter is in the correct spot
//   - Set the i-th character to Y if the guessed letter is in the word, but is 
//   not in the correct spot
//   - Set the i-th character to X if the guessed letter is not in the word
// effects: result is updated to contain a C string comprised of only G, Y and X
// requires: guess and wordle are valid 5-letter C strings (not asserted)
//           result has enough space to store a 5-letter C string (not asserted)
void process_guess(char guess[], char wordle[], char output[]) {
    assert(guess);
    assert(wordle);
    assert(output);

    for (int i = 0; i < WORD_LENGTH; ++i) {
        if (guess[i] == wordle[i]) {
            output[i] = 'G';
        } else if (contains_char(guess[i], wordle)) {
            output[i] = 'Y';
        } else {
            output[i] = 'X';
        }
    }
}
