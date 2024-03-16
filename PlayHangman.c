/*
 * Developer's Name: Aashish Maharjan
 * Version: 0.10.1
 * Last Updated: 5th May, 2019
 * Available on Github
 * Instagram: @aacismaharjan
*/

#include <stdio.h>
#include <conio.h>      // For getch() function
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define BOXLINES 70     // Total dotted lines in top and bottom
#define LIVES 6         // Total chances players get
#define PUNISH -50      // Score deduction value
#define REWARD 100      // Score increment value

// Function Declarations
void reset();
void pickWord();
int playAgain();
char guessWord();
void dashBoard();
int setBlanks(char);
int checkIfWon();
int isAlreadyGuessed(char);
int isInWord(char, char*);
void printMsg(char* word);
void dottedLines(unsigned int n);

// Global Variables
int position, length, scores;
char guess;

// Hangman board representation
char* board[] = {"", "|    O\n|       \n|       \n", "|    O\n|   /   \n|       \n", "|    O\n|   /|  \n|       \n", "|    O\n|   /|\\\n|       \n", "|    O\n|   /|\\\n|   /   \n", "|    O\n|   /|\\\n|   / \\\n"};
char* blanks = NULL;
char* secret = NULL;
char* guessingBank = NULL;

int main() {
    // Main game loop
    do {
        reset(); // Reset game variables
        while (position < LIVES) {
            system("cls");
            dashBoard();  // Display Hangman dashboard

            // Prompt for guessing a letter
            guessAgain:
            guess = guessWord();
            // Check if the letter has already been guessed
            if (isAlreadyGuessed(guess))
                goto guessAgain; // If guessed letter is repeated, prompt for input again
            // Check if the guessed letter is in the secret word
            else if (setBlanks(guess)) {
                // Check if the player has won
                if (checkIfWon())
                    break;
            } else
                // If guessed letter is not in the secret word, decrease the lives of the player
                guessingBank[position++] = guess;
        }

        // If the player lost the game
        if (position >= LIVES) {
            strcpy(blanks, secret);
            system("cls");
            dashBoard();
            printf("\n");
            printMsg("Try Again! You lost it man :(");
        }
    } while (playAgain()); // Prompt for a new game

    // Free dynamically allocated memory
    free(secret);
    free(blanks);
    free(guessingBank);
    return 0;
}

// Resets game variables
void reset() {
    position = scores = 0; // Reset position and scores
    pickWord(); // Pick a new word for the game
}

// Display Hangman dashboard
void dashBoard() {
    printMsg("HANGMAN: Recreated by Aashish Maharjan."); // Print game title
    printf("\n");

    dottedLines(BOXLINES); // Print dotted lines
    printf("Chances: %d\t\tHistory: [%s]\n", LIVES - position, guessingBank); // Display remaining chances and guessing history
    printf("______\n");
    printf("|    |\n");
    printf("%s", board[position]); // Display Hangman figure based on current position
    printf("|\n");
    printf("|\n");
    printf("------------------\n");
    printf("\nSecret: %s\t\tScores: %d\n", blanks, scores); // Display secret word and scores
    dottedLines(BOXLINES); // Print dotted lines
}

// Prompt for guessing a letter
char guessWord() {
    printf("\nGuess Letter: ");
    char c = getchar();
    if (getchar() != '\n') { // Check if more than one character is entered
        printf("\n");
        printMsg("Note: Please enter only one letter next time. It'll deduct scores!");
        scores += PUNISH; // Deduct scores for incorrect input
        while (getchar() != '\n'); // Flush input buffer
        return guessWord(); // Prompt for input again
    }
    if (isalpha(c)) // Check if entered character is an alphabet
        return tolower(c); // Return lowercase version of entered character
    return guessWord(); // Prompt for input again
}

// Check if the guessed letter has already been guessed
int isAlreadyGuessed(char guess) {
    if (isInWord(guess, guessingBank)) { // Check if guessed letter is in the guessing history
        printf("\n");
        printMsg("You have already entered that letter. Repetition will deduct scores!");
        scores += PUNISH; // Deduct scores for repeated guesses
        return 1; // Return true if letter is already guessed
    }
    return 0; // Return false if letter is not already guessed
}

// Check if secret word contains the guessed letter
int isInWord(char guess, char* word) {
    for (int i = 0; i < position; i++)
        if (word[i] == guess)
            return 1; // Return true if guessed letter is found in secret word
    return 0; // Return false if guessed letter is not found in secret word
}

// Fill blanks in the secret word with the guessed letter
int setBlanks(char guess) {
    int boolean = 0;
    for (int i = 0; i < length; i++) {
        if (secret[i] == guess) {
            blanks[i] = guess; // Fill blank space with guessed letter
            scores += REWARD; // Increment scores for correct guess
            boolean = 1;
        }
    }
    return boolean; // Return true if guessed letter is found in secret word
}

// Check if the player has won the game
int checkIfWon() {
    if (strcmp(secret, blanks) == 0) { // Compare secret word with revealed word
        system("cls");
        dashBoard();
        printMsg("You got it man!"); // Print win message
        return 1; // Return true if player has won
    }
    return 0; // Return false if player has not won yet
}

// Prompt for playing the game again
int playAgain() {
    printf("\nPress 'y' to continue: ");
    char c = getchar(); // Get user input
    while (getchar() != '\n'); // Flush input buffer
    system("cls"); // Clear screen
    return tolower(c) == 'y'; // Return true if user wants to play again, false otherwise
}

// Pick a random word from wordlist.txt
void pickWord() {
    FILE* file;
    int lines = 0;
    char c = 0;
    file = fopen("wordlist.txt", "rb"); // Open wordlist file in binary mode

    // Count total number of words in the file
    while ((c = getc(file)) != EOF)
        if (c == '\n')
            lines++;
    srand(time(NULL)); // Seed random number generator

    // Select a random line number
    int randomLine = 1 + rand() % lines;
    int characters = 0; // Initialize character count for the selected word
    int seekWhere = 0;  // Initialize seek position for fseek

    fseek(file, 0, SEEK_SET); // Move file pointer to the beginning

    lines = 0; // Reset line counter

    // Iterate through lines in the file
    while (lines < randomLine && (c = getc(file)) != EOF) {
        if (lines + 1 == randomLine)
            characters++; // Count characters for the selected word
        else
            seekWhere++; // Increment seek position
        if (c == '\n')
            lines++; // Increment line counter
    }

    // Adjust character count for carriage return and newline characters
    characters -= 2; // Contains '\r' and '\n' unnecessary characters (especially for Windows)
    length = characters; // Set the length of the secret word

    // Allocate memory for secret, blanks, and guessingBank
    if (secret == blanks == guessingBank == NULL) {
        secret = (char*)malloc(characters + 1); // Allocate memory for the secret word
        blanks = (char*)malloc(characters + 1); // Allocate memory for the blanks
        guessingBank = (char*)malloc(LIVES + 1); // Allocate memory for the guessing history
    } else {
        secret = realloc(secret, characters + 1); // Reallocate memory for the secret word
        blanks = realloc(blanks, characters + 1); // Reallocate memory for the blanks
    }

    // Set null terminator for secret, blanks, and guessingBank
    secret[characters] = blanks[characters] = guessingBank[6] = '\0';

    fseek(file, seekWhere, SEEK_SET); // Position file pointer to the beginning of the selected word
    fread(secret, 1, characters, file); // Read the selected word from file to secret
    for (int i = 0; i < length; i++)
        blanks[i] = '*'; // Initialize blanks with '*'
    for (int i = 0; i < LIVES; i++)
        guessingBank[i] = ' '; // Initialize guessingBank with whitespace

    fclose(file); // Close the file
}

// Print n dotted lines
void dottedLines(unsigned int n) {
    for (int i = 0; i < n; i++)
        printf("%c", '-');
    printf("\n");
}

// Print message with classic design
void printMsg(char* word) {
    dottedLines(BOXLINES);
    printf("%s\n", word);
    dottedLines(BOXLINES);
}
