/*
    * Developer's Name: Aashish Maharjan
    * Version: 0.10.1
    * Last Updated: 5th May, 2019
    * Available on Github
    * Instagram: @aacismaharjan
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define BOXLINES 70 // Total dotted lines in top and bottom
#define LIVES 6 // Total chances players get
#define PUNISH -50  // It deducts score
#define REWARD 100 // It increase score


// Function Declaration
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

char* board[] = {"", "|    O\n|       \n|       \n", "|    O\n|   /   \n|       \n", "|    O\n|   /|  \n|       \n", "|    O\n|   /|\\\n|       \n", "|    O\n|   /|\\\n|   /   \n", "|    O\n|   /|\\\n|   / \\\n"};
char* blanks = NULL;
char* secret = NULL;
char* guessingBank = NULL;

int main(){
    do{
        reset();
        //system("color F0"); // Enable to change theme
        while(position < LIVES){
            system("cls");
            dashBoard();  // Hangman DashBoard

            guessAgain:
            guess = guessWord();
            if(isAlreadyGuessed(guess)) goto guessAgain; // Checks if user repeated wrong word again
            else if(setBlanks(guess)){if(checkIfWon()) break;} // Fill blank space with guess
            else guessingBank[position++] = guess; // decrease the lives of user
        }

        // If user lost the game
        if(position >= LIVES){
            strcpy(blanks, secret);
            system("cls");
            dashBoard(); printf("\n");
            printMsg("Try Again! You lost it man :\(");
        }
    } while (playAgain()); // Prompts for new game

    // Free our pointers
    free(secret);
    free(blanks);
    free(guessingBank);
    return 0;
}

// Resets Game
void reset(){
    position = scores = 0;
    pickWord();
}

// Hangman Classic Dashboard
void dashBoard(){
    printMsg("HANGMAN: Recreated by Aashish Maharjan."); printf("\n");

    dottedLines(BOXLINES);
    printf("Chances: %d\t\tHistory: [%s]\n", LIVES-position, guessingBank);
    printf("______\n");
    printf("|    |\n");
    printf("%s", board[position]);
    printf("|\n");
    printf("|\n");
    printf("------------------\n");
    printf("\nSecret: %s\t\tScores: %d\n", blanks, scores);
    dottedLines(BOXLINES);
}

// Prompts for Guess Word
char guessWord(){
    printf("\nGuess Letter: ");
    char c = getchar();
    if(getchar()!='\n'){
        printf("\n");
        printMsg("Note: Please enter only one letter next time. It'll duduct scores!");
        scores+=PUNISH;
        while(getchar()!='\n');
        return guessWord();
    }
    if(isalpha(c)) return tolower(c);
    return guessWord();
}

// Checks if user repated letter
int isAlreadyGuessed(char guess){
    if(isInWord(guess, guessingBank)){
        printf("\n");
        printMsg("You have already entered that letter. Repetition will deduct scores!");
        scores+=PUNISH;
        return 1;
    } return 0;
}

// Check if secret contains guess
int isInWord(char guess, char* word){
    for(int i=0; i<position; i++) if(word[i] == guess) return 1;
    return 0;
}

// Fills the blanks with guess
int setBlanks(char guess){
    int boolean = 0;
    for(int i=0; i<length; i++){
        if(secret[i] == guess){
            blanks[i] = guess;
            scores+=REWARD;
            boolean = 1;
        }
    }
    return boolean;
}

// Checks if user wins the game
int checkIfWon(){
    if(strcmp(secret, blanks)==0){
        system("cls");
        dashBoard();
        printMsg("You got it man!");
        return 1;
    }
    return 0;
}

// Prompts for new game
int playAgain(){
    printf("\nPress 'y' to continue: ");
    char c = getchar();
    while(getchar()!='\n');
    system("cls");
    return tolower(c) == 'y';
}


// Randomly picks word from wordlist.txt
void pickWord(){
    FILE* file;
    int lines = 0;
    char c = 0;
    file = fopen("wordlist.txt", "r");

    // Gets total words
    while((c=getc(file))!=EOF) if(c=='\n') lines++;
    srand(time(NULL));

    int randomLine = 1 + rand() % lines;
    int characters = 0;
    int seekWhere = 0;

    fseek(file, 0, SEEK_SET);
    lines = 0;

    // Gets character length and position
    while(lines < randomLine && (c=getc(file))!=EOF){
        if(lines+1 == randomLine) characters++;
        else seekWhere++;
        if(c=='\n') lines++;
    }

    characters--;
    length = characters;

    // Dynamic Memory Allocation
    if(secret==blanks==guessingBank==NULL){
        secret = (char*) malloc(characters+1);
        blanks = (char*) malloc(characters+1);
        guessingBank = (char*) malloc(6);
    }else{
        secret = realloc(secret, characters+1);
        blanks = realloc(blanks, characters+1);
    }

    secret[characters] = blanks[characters] = guessingBank[6] = '\0'; // String is ended with '\0'
    fseek(file, seekWhere, SEEK_SET); // Position to secret word
    fread(secret, 1, characters, file); // Puts word from file to secret
    for(int i=0; i<length; i++) blanks[i] = '*'; // Filling Blanks with '*'
    for(int i=0; i<LIVES; i++) guessingBank[i] = ' '; // Filling GuessingBank with whitespace
    fclose(file);
}


// Prints n dotted lines
void dottedLines(unsigned int n){
    for(int i=0; i<n; i++) printf("%c", '-'); printf("\n");
}

// Prints with classic  design
void printMsg(char* word){
    dottedLines(BOXLINES);
    printf("%s\n", word);
    dottedLines(BOXLINES);
}
