/* Name:    Kevin Takamura */
/* Date:    April 28, 2025 */
/* Purpose: Project 10     */

#include <stdio.h>
#include <stdbool.h>

// Define macros for use in arrays and functions
#define FILE_IN "word.txt"
#define WORD_LIMIT 25
#define GUESS_LIMIT 6
#define STR_LIMIT 25
#define SELECTED_WORD 0

// Declare functions:
// Get word to be guessed from file, return number of words
int getWords(FILE *fileAddress, int lim, char list[][STR_LIMIT]);
// Verify that user's word input does not exceed 5 characters and does not contain non-letters; also converts capital letters to lowercase if there are no matches
_Bool checkGuess(int guess, int wordSel, char words[][STR_LIMIT], char str[][STR_LIMIT], char symb[][STR_LIMIT]);
// Get user's guess
void getGuess(int guess, char str[][STR_LIMIT]);
// Check user's guess for matches
void inspectGuess(int guess, char str[][STR_LIMIT]);
// Print user's guesses with hints and matching letters
void printGuesses(int guess, char str[][STR_LIMIT], char symb[][STR_LIMIT]);
// Display win message
void win(int guess, char word[][STR_LIMIT]);
// Display loss message
void loss();

int main(){
	// Declare local variables
	FILE *wordfile;
	char wordList[WORD_LIMIT][STR_LIMIT], guessList[GUESS_LIMIT][STR_LIMIT], hintList[GUESS_LIMIT][STR_LIMIT];
	int numGuess = 0, numWords, randomNumber;
	_Bool runGame = 1, winGame = 0;
	
	// Open file containing words to be guessed
	wordfile = fopen(FILE_IN, "r");
	// If file could not be opened, end with error; otherwise, continue with game
	if(wordfile == NULL){
		printf("Couldn't open file");
	} else{
		// Get number of words and write words to wordList array
		numWords = getWords(wordfile, WORD_LIMIT, wordList);
		
		// Loop the guessing logic until the user runs out of guesses or guesses the word correctly
		while(numGuess < GUESS_LIMIT && winGame == 0){
			// Increment guess number every time the loop repeats
			numGuess++;
			// Get user input
			getGuess(numGuess, guessList);
			// Check for non-letters and string length
			inspectGuess(numGuess, guessList);
			// Check for character matches; if the whole string matches, end the while loop
			winGame = checkGuess(numGuess, SELECTED_WORD, wordList, guessList, hintList);
			// Only print guesses with hints if the user did not win
			if(winGame == 0){
				printGuesses(numGuess, guessList, hintList);
			}
		}
		
		// If the user won the game, display win message; otherwise, display loss message
		if(winGame == 1){
			win(numGuess, guessList);
		} else{
			loss();
		}
		
	}
	
	return 0;
}

int getWords(FILE *fileAddress, int lim, char list[][STR_LIMIT]){
	int counter = 0;
	// Increment counter for every successful write to the 2D array
	while(fscanf(fileAddress, "%s", list[counter]) == 1 && counter < lim){
		counter++;
	}
	
	// Return counter value
	return counter;
}

void getGuess(int guess, char str[][STR_LIMIT]){
	int guessIndex = guess - 1;
	char guessString[STR_LIMIT];
	
	// If the user has more than 2 guesses remaining, print normal guess prompt; otherwise, print final guess prompt
	if(guess < 6){
		printf("GUESS %d! Enter your guess: ", guess);
	} else{
		printf("FINAL GUESS: ");
	}
	
	// Get user string input
	fgets(guessString, STR_LIMIT, stdin);
	
	// Copy input to parameter array
	for(int i = 0; guessString[i] != '\0'; i++){
		str[guessIndex][i] = guessString[i];
	}
	
}

void inspectGuess(int guess, char str[][STR_LIMIT]){
	_Bool repeatGuess = 1, symbolFound = 0;
	char guessString[STR_LIMIT];
	int stringLength = 0;
	int guessIndex = guess - 1;
	
	// Copy parameter array to temporary array
	for(int i = 0; guessString[i] != '\0'; i++){
		guessString[i] = str[guessIndex][i];
	}
	
	// Repeat logic until the user enters a satisfactory value (exactly 5 letters)
	do{
		// Go through the entire array checking for letters; if they are present, increment a counter and add a null character to the end; if there are non-letters, switch a _Bool
		for(int i = 0; guessString[i] != '\0'; i++){
			if((guessString[i] >= 'A' && guessString[i] <= 'Z') 
			|| (guessString[i] >= 'a' && guessString[i] <= 'z')){
				stringLength++;
			} else{
				if(guessString[i] != '\n'){
					symbolFound = 1;
				} else{
					guessString[i] = '\0';
				}
			}
		}
		
		// If the string length calculated was not exactly 5 and there are no symbols, re-prompt with invalid character count error
		if(stringLength != 5 && symbolFound == 0){
			printf("Your guess must be 5 letters long.");
			printf("\nPlease try again: ");
			// Reset _Bool
			stringLength = 0;
			
			// Clear temporary array
			for(int i = 0; i < STR_LIMIT; i++){
				guessString[i] = '\0';
			}
			
			// Retry user string input and re-run logic
			fgets(guessString, STR_LIMIT, stdin);
			
		// If a symbol was found regardless of string length, re-prompt with invalid input error
		} else if(symbolFound == 1){
			printf("Your guess must only contain letters.");
			printf("\nPlease try again: ");
			// Reset _Bools
			stringLength = 0;
			symbolFound = 0;
			
			// Clear temporary array
			for(int i = 0; i < STR_LIMIT; i++){
				guessString[i] = '\0';
			}
			
			// Retry user string input and re-run logic
			fgets(guessString, STR_LIMIT, stdin);
		
		// If input is satisfactory, check for capital letters; if present, change to lowercase
		}else{
			for(int i = 0; guessString[i] != '\0'; i++){
				if(guessString[i] >= 'A' && guessString[i] <= 'Z'){
					guessString[i] = guessString[i] + 32;
				}
				
				// Then, write satisfactory input to parameter array and exit function
				str[guessIndex][i] = guessString[i];
				repeatGuess = 0;
			}
		}
		
	} while(repeatGuess == 1);
}

_Bool checkGuess(int guess, int wordSel, char words[][STR_LIMIT], char str[][STR_LIMIT], char symb[][STR_LIMIT]){
	int guessIndex = guess - 1;
	// Count number of correctly placed letters (shown as capital letters)
	int charCounter = 0;
	
	// Initialize all hint parameter array values as spaces
	for(int i = 0; i < 5; i++){
		symb[guessIndex][i] = 32;
	}
	
	// Set 6th index as null character
	symb[guessIndex][5] = '\0';
	
	// Check for exact matches per character; if present, set character as a capital letter and increment counter
	for(int i = 0; str[guessIndex][i] != '\0'; i++){
		if(words[wordSel][i] == str[guessIndex][i]){
			str[guessIndex][i] = str[guessIndex][i] - 32;
			charCounter++;
		}
	}
	
	// If the whole string matches, exit function with win condition; else, continue
	if(charCounter == 5){
		return 1;
	}
	
	// Check for misplaced matches; if present, show a hint marker underneath the corresponding character
	for(int i = 0; str[guessIndex][i] != '\0'; i++){
		for(int j = 0; words[wordSel][j] != '\0'; j++){
			if(str[guessIndex][i] == words[wordSel][j] && str[guessIndex][i] != words[wordSel][i]){
				symb[guessIndex][i] = '^';
			}
		}
	}
	
	// Exit function with non-win condition
	return 0;
}


void printGuesses(int guess, char str[][STR_LIMIT], char symb[][STR_LIMIT]){
	printf("================================\n");
	
	// For all existing guesses, print string with corresponding hints
	for(int i = 0; i < guess; i++){
		for(int j = 0; j < 5; j++){
			printf("%c", str[i][j]);
		}
		printf("\n");
		for(int j = 0; j < 5; j++){
			printf("%c", symb[i][j]);
		}
		printf("\n");
	}
}

void win(int guess, char word[][STR_LIMIT]){
	int guessIndex = guess - 1;
	printf("================================\n		");
	// Print correct word
	for(int i = 0; i < 5; i++){
		printf("%c", word[guessIndex][i]);
	}
	printf("\n");
	
	// Print win message for corresponding number of guesses
	if(guess == 1){
		printf("	You won in 1 guess!\n");
	} else{
		printf("	You won in %d guesses!\n", guess);
	}
	
	if(guess == 1){
		printf("		GOATED!\n");
	} else if(guess > 1 && guess < 4){
		printf("		Amazing!\n");
	} else if(guess >= 4 && guess < 6){
		printf("		Nice!\n");
	}
}

void loss(){
	// Print unconditional loss message
	printf("You lost, better luck next time!\n");
}
	
