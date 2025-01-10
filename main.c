#include "wordle_solver.h"
#include <stdio.h>
#include <string.h>


int main(void) {
    char dict[3103][6] = {{0}};
    load_words("words.txt", dict);
    char wordle[6];
    pick_word(wordle, dict, 3103);
    char result[6] = "XXXXX";
    struct word_analysis analysis = { { { '\0',""} }, "" };
    while(strcmp(result,"GGGGG") != 0) {
        char guess[6] = {0};
		printf("Enter a guess:\n");
		auto_select(guess,dict,DICT_LENGTH,&analysis);
		printf("%s\n",guess);
		process_guess(guess,wordle,result);
		analyze_result(guess,result, &analysis); 
		printf("Result: %s\n",result);
    }
}