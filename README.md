# HangmanHell
This is a text-based word game in which a random word is selected, and the user guesses letters until the word is revealed, similar to Hangman. However, there's a dark twist: the program changes the target word as the game goes on, such that the user is incorrect as much as possible.
The algorithm considers every word as an option, and as the user makes guesses, the pool of potential target words becomes smaller to make the player's guess incorrect. However, there is only a finite number of times this algorithm can be applied, and thus it is possible to win every time. 
This program was written for CS211 in Fall of 2023 at UIC.
