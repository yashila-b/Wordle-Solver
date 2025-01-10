# Wordle Solver in C
A fully functional Wordle solver implemented in C. The program automatically selects valid guesses based on positional and character restrictions, optimizing gameplay using a dictionary of over 10,000 words.

## Features
- Dynamic word filtering based on game feedback.
- Efficient use of data structures like arrays and structs.
- Automated word selection and result analysis.
- File I/O for loading and managing the word dictionary.

## How to Run
1. Compile the code using a C compiler (e.g., `gcc wordle_solver.c -o wordle_solver`).
2. Place the `words.txt` file in the same directory as the executable.
3. Run the program and follow the instructions.

## Example Output
Enter a guess:
aback
Result: XXXXX
Enter a guess:
deeds
Result: XYYXX
Enter a guess:
eight
Result: YYXXX
Enter a guess:
flier
Result: GGGGG
