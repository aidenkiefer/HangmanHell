#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Pattern_struct {
    char* pat;  //string pattern - exs: "hello", "-ell-", "-----"
    int count;  //frequency of pattern in the current word list
    int changes;//number of occurences of new letter in this pattern
} Pattern;

//VII
void updateWordPattern(const char* targetWord, char* wordPattern, char guess, int remainingGuesses) {
    bool goodGuess = false;
    for (int i = 0; targetWord[i] != '\0'; i++) {
        if (targetWord[i] == guess) {
            wordPattern[i] = guess;
            goodGuess = true;
        }
    }
    if (goodGuess) {
        printf("Good guess! The word has at least one %c\n", guess);
    }
    else {
        printf("Oops, there are no %c's. You used a guess.\n", guess);
        remainingGuesses--;
    }
}

bool isValid(char guess, char* wordPattern){
    if (!islower(guess)){
        return false;
    }
    else if (strchr(wordPattern, guess)) {
        return false;
    }
    else if (!isalpha(guess)){
        return false;
    }
    return true;
}

//II -- addWord function adds words to dictionary
void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
    // check if current number of words is at or above
    //max word capacity
    if (*numWords >= *maxWords) {
        //double capacity
        *maxWords *= 2;
        //realloc memory for array of C-string pointers
        *words = (char**)realloc(*words, (*maxWords) * sizeof(char*));
        if (*words == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }

    //malloc for new word
    (*words)[*numWords] = (char*)malloc((strlen(newWord) + 1) * sizeof(char));
    if ((*words)[*numWords] == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    //copy
    strcpy((*words)[*numWords], newWord);
    //increment number of words
    (*numWords)++;
}

//III
int strNumMods(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int maxLen = len1 > len2 ? len1 : len2; // get max length

    int numDifferences = 0;

    // iterate thru strings up to length of longer string
    for (int i = 0; i < maxLen; i++) {
        if (i >= len1 || i >= len2 || str1[i] != str2[i]) {
            numDifferences++;
        }
    }

    return numDifferences;
}

//VI
int strDiffInd(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int maxLen = len1 > len2 ? len1 : len2; // get max length

    for (int i = 0; i < maxLen; i++) {
        if (i >= len1 || i >= len2 || str1[i] != str2[i]) {
            return i;
        }
    }

    // if both strings are the same return their length
    return len1;
}

int main(int argc, char* argv[]) {

    printf("Welcome to the (Evil) Word Guessing Game!\n\n");
    
    //Task II vars
    int numWords = 0;

    //Task III vars
    int maxWords = 4;
    char** wordList = (char**)malloc(maxWords*sizeof(char*));
    int totalWords = 0;
    int longestWordLength = 0;
    
    //Task I - assign game settings based on args
    //initialize variables
    int wordSize = 5;
    int numGuesses = 26;
    int statsMode = 0;
    int wordListMode = 0;
    int letterListMode = 0;
    int patternListMode = 0;
    //check args
    for (int i = 1; i < argc; i++){
        //wordSize args
        if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                wordSize = atoi(argv[i+1]);
                if (wordSize < 2 || wordSize > 29) {
                    printf("Invalid word size.\nTerminating program...\n");
                    exit(1);
                }
                i++; //skip
            }
            else {
                printf("Missing argument after -n.\nTerminating program...\n");
                exit(1);
            }
        }
        //numGuesses args
        else if (strcmp(argv[i], "-g") == 0) {
            if (i + 1 < argc) {
                numGuesses = atoi(argv[i + 1]);
                if (numGuesses <= 0){
                    printf("Invalid number of guesses.\nTerminating program...\n");
                    exit(1);
                }
                i++;
            }
            else {
                printf("Missing argument after -g.\nTerminating program...\n");
                exit(1);
            }
        }
        //Mode args
        else if (strcmp(argv[i], "-s") == 0) {
            statsMode = 1;
        }
        else if (strcmp(argv[i], "-w") == 0) {
            wordListMode = 1;
        }
        else if (strcmp(argv[i], "-l") == 0) {
            letterListMode = 1;
        }
        else if (strcmp(argv[i], "-p") == 0) {
            patternListMode = 1;
        }
        else if (strcmp(argv[i], "-v") == 0) {
            statsMode = 1;
            wordListMode = 1;
            letterListMode = 1;
            patternListMode = 1;
        }
        else {
            printf("Invalid command-line argument.\nTerminating program...\n");
            exit(1);
        }
    }


    //print game settings
    printf("Game Settings:\n");
    printf("  Word Size = %d\n", wordSize);
    printf("  Number of Guesses = %d\n", numGuesses);
    printf("  View Stats Mode = %s\n", statsMode ? "ON" : "OFF");
    printf("  View Word List Mode = %s\n", wordListMode ? "ON" : "OFF");
    printf("  View Letter List Mode = %s\n", letterListMode ? "ON" : "OFF");
    printf("  View Pattern List Mode = %s\n", patternListMode ? "ON" : "OFF");

    
    //Task III - open file and parse data accordingly
    FILE *file = fopen("dictionary.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open dictionary file.\n");
        return 1;
    }

    char word[30];
    char longestWord[30];
    while (fscanf(file, "%s", word) == 1) {
        totalWords++;
        //update longestWordLength
        if (strlen(word) > longestWordLength) {
            longestWordLength = strlen(word);
            strcpy(longestWord, word);
        }
        //check if word length == wordSize
        if (strlen(word) == wordSize) {
            //check if more room is necessary
            if (numWords >= maxWords) {
                maxWords *= 2;
                wordList = realloc(wordList, maxWords * sizeof(char *));
                if (wordList == NULL) {
                    printf("Error: Memory allocation failed.\n");
                    fclose(file);
                    return 1;
                }
            }

            //add word to wordList
            addWord(&wordList, &numWords, &maxWords, word);
        }
    }

    fclose(file);
    

    //Task IV - check mode and print data accordingly
    //check if stat mode is on
    if (statsMode) {
        //print dictionary stats
        printf("The dictionary contains %d words total.\n", totalWords);
        printf("The longest word %s has %d chars.\n", longestWord, longestWordLength);
        printf("The dictionary contains %d words of length %d.\n", numWords, wordSize);
        printf("Max size of the dynamic words array is %d.\n", maxWords);
    }

    if (wordListMode) {
        printf("Words of length %d:\n", wordSize);
        if (numWords == 0) {
            printf("Dictionary has no words of length %d.\nTerminating program...\n", wordSize);
            exit(1);
        } else {
            for (int i = 0; i < numWords; i++) {
                printf("  %s\n", wordList[i]);
            }
        }
    }
    
    //VII
    //Task VII vars
    //dynamically allocate memory for targetWord and wordPattern
    char* targetWord = (char*)malloc((wordSize + 1) * sizeof(char));
    char* wordPattern = (char*)malloc((wordSize + 1) * sizeof(char));
    

    if (targetWord == NULL || wordPattern == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    //initialize targetWord with your chosen word
    strcpy(targetWord, "sample"); //replace with your actual target word

    //initialize wordPattern with dashes
    for (int i = 0; i < wordSize; i++) {
        wordPattern[i] = '-';
    }
    wordPattern[wordSize] = '\0'; //null-terminate the string

    char guessedLetters[26]; //to store guessed letters (a-z)
    int numGuessedLetters = 0;

    int remainingGuesses = numGuesses;
    bool solved = false;

    Pattern* patterns = (Pattern*)malloc(4 * sizeof(Pattern));
    int patternCount = 0;
    int patternCapacity = 4;

    printf("The word pattern is: %s\n\n", wordPattern);
    while (remainingGuesses != 0 && !solved) {
        char guess;
        printf("Number of guesses remaining: %d\n", remainingGuesses);
        if (letterListMode){
            printf("Previously guessed letters: ");
            for (int i = 0; i < numGuessedLetters; i++) {
                printf("%c", guessedLetters[i]);
                if (i < numGuessedLetters - 1) {
                    printf(" ");
                }
            }
        }
        printf("\nGuess a letter (# to end game): ");
        scanf(" %c", &guess);
        printf("\n");
        if (guess == '#') {
            break;
        }
        else if (!isValid(guess, wordPattern)) {
            printf("Invalid letter...\n");
            continue;
        }
        guessedLetters[numGuessedLetters++] = guess;

        bool newPattern = true;
        char* updatedWordPattern = (char*)malloc((wordSize + 1) * sizeof(char));
        if (updatedWordPattern == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }
        strcpy(updatedWordPattern, wordPattern);
        
        //create updated pattern based on current guessed letter
        for (int i = 0; i < wordSize; i++) {
            if (targetWord[i] == guess) {
                updatedWordPattern[i] = guess;
            }
        }
        
        //check if updated pattern is unique
        for (int i = 0; i < patternCount; i++) {
            if (strcmp(updatedWordPattern, patterns[i].pat) == 0) {
                newPattern = false;
                patterns[i].count++;
                patterns[i].changes++;
                break;
            }
        }
        
        //add the unique pattern to dynamic array
        if (newPattern) {
            if (patternCount >= patternCapacity) {
                patternCapacity *= 2;
                patterns = (Pattern*)realloc(patterns, patternCapacity * sizeof(Pattern));
                if (patterns == NULL) {
                    printf("Memory allocation failed.\n");
                    return 1;
                }
            }
            strcpy(patterns[patternCount].pat, updatedWordPattern);
            patterns[patternCount].count = 1;
            patterns[patternCount].changes = 0;
            patternCount++;
        }

        //find most common pattern (with tiebreakers)
        int mostCommonPatternIndex = 0;
        for (int i = 1; i < patternCount; i++) {
            if (patterns[i].count > patterns[mostCommonPatternIndex].count ||
                (patterns[i].count == patterns[mostCommonPatternIndex].count &&
                patterns[i].changes < patterns[mostCommonPatternIndex].changes) ||
                (patterns[i].count == patterns[mostCommonPatternIndex].count &&
                patterns[i].changes == patterns[mostCommonPatternIndex].changes &&
                strDiffInd(patterns[i].pat, updatedWordPattern) < strDiffInd(patterns[mostCommonPatternIndex].pat, updatedWordPattern))) {
                mostCommonPatternIndex = i;
            }
        }

        //update and print final word pattern
        updateWordPattern(targetWord, wordPattern, guess, remainingGuesses);
        printf("The word pattern is: %s\n\n", wordPattern);

        //check if solved
        if (strcmp(wordPattern, targetWord) == 0) {
            solved = true;
            break;
        }
        free(updatedWordPattern);
    }

    //test adding words
    //addWord(&wordList, &numWords, &maxWords, "apple");
    //addWord(&wordList, &numWords, &maxWords, "banana");


    //free wordList
    for (int i = 0; i < numWords; i++) {
        free(wordList[i]);
    }
    free(wordList);
    free(targetWord);
    free(wordPattern);
    for (int i = 0; i < patternCount; i++) {
        free(patterns[i].pat);
    }
    free(patterns);

    
    return 0;
}
