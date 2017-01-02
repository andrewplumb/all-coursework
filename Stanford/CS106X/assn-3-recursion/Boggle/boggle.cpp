/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */
 
#include <cctype>
#include <iostream>
using namespace std;

#include "simpio.h"
#include "gwindow.h"
#include "gboggle.h"
#include "random.h"
#include "set.h"
#include "lexicon.h"

const int kBoggleWindowWidth = 650;
const int kBoggleWindowHeight = 350;
static const string kEnglishLanguageDatafile = "EnglishWords.dat";

const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static void welcome();
static bool responseIsAffirmative(const string& prompt);
static void giveInstructions();
static Vector<Vector<char> > boardCreation();
static bool useBigBoard();
static bool useUserGeneratedBoard();
static Vector<Vector<char> > generateUserSpecifiedBoard(int boardDimension);
static Vector<Vector<char> > fillBoard(int boardDimension, string boardContents);
static Vector<string> shuffleCubes(int boardDimension);
static string createRandomContents(Vector<string> cubes);
static void playersTurn(Lexicon& english, Vector<Vector<char> > board, Set<string>& usedWords);
static bool guessOnBoard(string guess, Vector<Vector<char> > board);
static bool searchForGuess(string guess, Vector<Vector<char> > board, string guessFound, string guessLeft, Set<string> coordsUsed, int row, int col);
static void clearColors(Vector<Vector<char> > board);
static void computersTurn(Lexicon english, Vector<Vector<char> > board, Set<string>& usedWords);
static bool findWords(Lexicon english, Vector<Vector<char> > board, string prefix, Set<string> coordsUsed, int row, int col, Set<string>& usedWords);


int main() {
    Lexicon english(kEnglishLanguageDatafile);
    GWindow gw(kBoggleWindowWidth, kBoggleWindowHeight);
    initGBoggle(gw);
    while(true){
        welcome();
        if (responseIsAffirmative("Do you need instructions? ")) {
            giveInstructions();
        }
        Vector<Vector<char> > board = boardCreation();
        Set<string> usedWords;
        playersTurn(english, board, usedWords);
        computersTurn(english, board, usedWords);
        if (!responseIsAffirmative("Would you like to play again?")) break;
    }
    return 0;
}

/*
 * function: welcome()
 * usage: welcome()
 * -------------------
 * Welcomes the user to the program and introduces the idea of the game.
 */
static void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * function: responseIsAffirmative(const string& prompt)
 * usage: if (responseIsAffirmative(prompt)) ...
 * -------------------
 * Prints prompt to the screen, and asks for user input of yes or no.
 * If yes, returns true, if no, returns false.
 */
static bool responseIsAffirmative(const string& prompt) {
    while (true) {
        string answer = getLine(prompt);
        if (!answer.empty()) {
            switch (toupper(answer[0])) {
                case 'Y': return true;
                case 'N': return false;
            }
        }
        cout << "Please answer yes or no." << endl;
    }
}

/*
 * function: giveInstructions()
 * usage: giveInstructions()
 * -------------------
 * Prints the instructions to the game of Boggle, and waits for
 * the user to hit enter to continue.
 */
static void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

/*
 * function: boardCreation()
 * usage: boardCreation()
 * -------------------
 * Creates a boggle board that is either 4x4 or 5x5, with inputs randomized from
 * the boggle game or input by the user, depending on user choices.
 */
static Vector<Vector<char> > boardCreation(){
    int boardDimension = 4;
    if (useBigBoard()) boardDimension = 5;
    drawBoard(boardDimension, boardDimension);
    if (useUserGeneratedBoard()){ // Generates the board from the user's input
        Vector<Vector<char> > board = generateUserSpecifiedBoard(boardDimension);
        return board;
    }
    else { // Generates the board from the boggle cubes
        Vector<string> cubes = shuffleCubes(boardDimension);
        string boardContents = createRandomContents(cubes);
        Vector<Vector<char> > board = fillBoard(boardDimension, boardContents);
        return board;
    }
}

/*
 * function: useBigBoard()
 * usage: if (useBigBoard()) ...
 * -------------------
 * Asks the user if they would like to have the boggle board be a big board (5x5).
 */
static bool useBigBoard(){
    cout << "You can choose standard Boggle (4x4 grid)" << endl;
    cout << "or Big Boggle (5x5)." << endl;
    return responseIsAffirmative("Would you like Big Boggle?");
}

/*
 * function: useUserGeneratedBoard()
 * usage: if (useUserGeneratedBoard()) ...
 * -------------------
 * Asks the user if they would like to input their own board.
 */
static bool useUserGeneratedBoard(){
    cout << "I'll give you a chance to set up the board to your specifications, which is the only " << endl;
    cout << "way you may possibly come close to the computers score." << endl;
    return responseIsAffirmative("Do you want to force the board configuration?");
}

/*
 * function: Vector<Vector<char> > generateUserSpecifiedBoard(int boardDimension)
 * usage: board = generateUserSpecifiedBoard(boardDimension)
 * -------------------
 * Generates a board from a user-inputed string, and returns Vector<Vector<char> > board which contains
 * the elements of the board.
 */
static Vector<Vector<char> > generateUserSpecifiedBoard(int boardDimension){
    cout << "Enter a " << boardDimension*boardDimension << "-character string to identify which letters you want on the cubes." << endl;
    cout << "the first " << boardDimension << " letters are the cubes on the top row from left to right, the next ";
    cout << boardDimension << " letters are the second row, and so on." << endl;
    while (true){
        string userBoard = getLine("Enter the String:");
        int i = 0;
        while (userBoard[i]){                       // converts the user's string to all upper case
            userBoard[i] = toupper(userBoard[i]);
            i++;
        }
        if (userBoard.length() >= boardDimension*boardDimension){               // if the string has sufficient letters,
            Vector<Vector<char> > board = fillBoard(boardDimension, userBoard); // fills the board with the users string.
            return board;
        }
        else cout << "That wasn't enough letters." << endl;
    }
}

/*
 * function: Vector<Vector<char> > fillBoard(int boardDimension, string boardContents){
 * usage: board = fillBoard(boardDimension, boardContents)
 * -------------------
 * Displays the values in string boardContents on the graphic window for the boggle board,
 * and fills Vector<Vector<char> > with these values.
 */
static Vector<Vector<char> > fillBoard(int boardDimension, string boardContents){
    int i = 0;
    Vector<char> currentRow;
    Vector<Vector<char> > board;
    for (int row = 0; row < boardDimension; row++){
        currentRow.clear();
        for (int col = 0; col < boardDimension; col++){
            labelCube(row, col, boardContents[i]);     // labels the cube at row, col with boardContents
            currentRow.add(boardContents[i]);          // adds board contents to the active row.
            i++;
        }
        board.add(currentRow);                          // fills board with vectors currentRow.
    }
    return board;
}

/*
 * function: Vector<string> shuffleCubes(int boardDimension)
 * usage: cubes = shuffleCubes(boardDimension)
 * -------------------
 * Shuffles the strings in kStandardCubes or kBigBoggleCubes, and stores them
 * in Vector<string> cubes.
 */
static Vector<string> shuffleCubes(int boardDimension){
    Vector<string> cubes;
    if (boardDimension == 4){               // Adds the standard cubes to cubes
        for (int i = 0; i < 16; i++){
            cubes.add(kStandardCubes[i]);
        }
    }else{
        for (int i = 0; i < 25; i++){       // Adds the bigBoggle cubes to cubes
            cubes.add(kBigBoggleCubes[i]);
        }
    }
    for (int i = 0; i < cubes.size()-1; i++){ // Shuffles cubes.
        swap(cubes[i], cubes[randomInteger(i, cubes.size()-1)]);
    }
    return cubes;
}

/*
 * function: string createRandomContents(Vector<string> cubes)
 * usage: boardContents = createRandomContents(cubes)
 * -------------------
 * Takes the cubes Vector, and takes a random value from each cube
 * and adds it to board contents.
 */
static string createRandomContents(Vector<string> cubes){
    string boardContents = "";
    for (int i = 0; i < cubes.size(); i++){                             // for each cube string in cubes
        boardContents += cubes[i][randomInteger(0,cubes[i].length()-1)];// adds a random value from that
    }                                                                   // string to boardContents.
    return boardContents;
}

/*
 * function:  void playersTurn(Lexicon& english, Vector<Vector<char> > board, Set<string>& usedWords)
 * usage: playersTurn(english, board, usedWords)
 * -------------------
 * Plays the human players turn.  Takes an input guess from the user, and checks to ensure the length
 * is greater than 4 and an english word that hasn't been guessed yet.  If the guess is legal,
 * the board is searched for the guess.  If the guess is found, it is added to the scoreboard and
 * Set<string> usedWords.
 */
static void playersTurn(Lexicon& english, Vector<Vector<char> > board, Set<string>& usedWords){
    cout << "Ok, take all the time you want and find all the words you can! Signal that you're" << endl;
    cout << "finsed by entering an empty line." << endl;
    Set<string> pastGuesses;
    while (true){
        string guess = getLine("Enter a word: ");
        int i = 0;
        while (guess[i]){
            guess[i] = toupper(guess[i]);
            i++;
        }
        if (guess == "") break;
        if (guess.length() < 4){                                    // Checks to make sure the guess is legal
            cout << "Guess must be at least 4 letters." << endl;
        }else if(!english.contains(guess)){
            cout << "You must guess english words." << endl;
        }else if(pastGuesses.contains(guess)){
            cout << "You've already made that guess." << endl;
        }else{
            pastGuesses.add(guess);
            if (guessOnBoard(guess, board)){        // If the guess is on the board, record it for the human player.
                cout << "Good job! " << guess << " can be found and is worth " << guess.length()-3;
                if (guess.length()-3 == 1){
                    cout << " point!" << endl;
                } else{
                    cout << " points!" << endl;
                }
                clearColors(board);
                recordWordForPlayer(guess, HUMAN);
                usedWords.add(guess);
            }
            else cout << "You can't make that word!" << endl;
        }
    }
}

/*
 * function: bool guessOnBoard(string guess, Vector<Vector<char> > board)
 * usage: if (guessOnBoard(guess, board)) ...
 * -------------------
 * Loops through the board, looking for the first letter of the guess.  If the first
 * letter is found, searchForGuess is called to see if the rest of the guess can be
 * found.  Returns true if the guess is on the board.
 */
static bool guessOnBoard(string guess, Vector<Vector<char> > board){
    string guessFound = "";
    Set<string> coordsUsed;
    string guessLeft = guess;
    for (int row = 0; row < board.size(); row++){       // loops through all locations looking
        for (int col = 0; col < board.size(); col++){   // for the first letter of the guess
            if (board[row][col] == guess[0]){           // If it's found, call searchForGuess.
                if (searchForGuess(guess, board, guessFound, guessLeft, coordsUsed, row, col)){
                    highlightCube(row, col, true);      // If searchForGuess returns true, highlight
                    return true;                        // row, col as the first letter of the guess.
                }
            }
        }
    }
    return false;
}

/*
 * function:bool searchForGuess(guess, board, guessFound, guessLeft, coordsUsed, row, col)
 * usage: if (searchForGuess(guess, board, guessFound, guessLeft, coordsUsed, row, col)) ...
 * -------------------
 * Recursively searches neighbors of the last letter of the guess that has been found so far, ignoring locations
 * that have already been used to formulate the guess so far.  If the next letter in the guess is a neighbor of the
 * last found letter in the guess, searchForGuess is called again. If the guess found so far is the whole guess, it
 * returns true and colors the locations that make up the guess.
 */
static bool searchForGuess(string guess, Vector<Vector<char> > board, string guessFound, string guessLeft, Set<string> coordsUsed, int row, int col){
    coordsUsed.add(integerToString(row)+integerToString(col)); // coordsUsed stores strings of row+col, so if the upperleft cube was used,
    guessFound += guessLeft[0];                                // coordsUsed would contain the string "00"
    if (guessFound == guess) return true;
    guessLeft = guessLeft.substr(1);
    if (guessLeft == "") return false;
    for (int i = -1; i <= 1; i++){                                                          // loop through the neighbors of the last
        for (int j = -1; j <= 1; j++){                                                      // found letter in the guess.
            if (0 <= row+i && row+i < board.size() && 0 <= col+j && col+j < board.size() && // only check if the board location is the
                    !coordsUsed.contains(integerToString(row+i)+integerToString(col+j))){   // next letter if it hasn't been used yet
                if(board[row+i][col+j] == guessLeft[0]){                                    // and is on the board. If a neighbor
                    if (searchForGuess(guess, board, guessFound, guessLeft, coordsUsed, row+i, col+j)){ // has the next letter in the
                        highlightCube(row+i, col+j, true);                                  // guess, call searchForGuess with it's location.
                        return true;                                                        // If the guess is found, the locations that
                    }                                                                       // The guess is found in are highlighted.
                }
            }
        }
    }
}

/*
 * function: clearColors(Vector<Vector<char> > board)
 * usage: clearColors(board)
 * -------------------
 * After a guess had been found, the locations that make up the guess are highlighted.  This clears
 * the highlighting after waiting half a second.
 */
static void clearColors(Vector<Vector<char> > board){
    pause(500);
    for (int row = 0; row < board.size(); row++){
        for (int col = 0; col < board.size(); col++){
            highlightCube(row, col, false);
        }
    }
}

/*
 * function:void computersTurn(Lexicon english, Vector<Vector<char> > board, Set<string>& usedWords)
 * usage:computersTurn(english, board, usedWords)
 * -------------------
 * Runs the computers turn.  The computer loops through each board location, and finds all legal english
 * words starting at that location that haven't been found yet by either the player or the computer.
 */
static void computersTurn(Lexicon english, Vector<Vector<char> > board, Set<string>& usedWords){
    Set<string> coordsUsed;
    for (int row = 0; row < board.size(); row++){       // loops through every board location,
        for (int col = 0; col < board.size(); col++){   // and searches for words starting at
            string prefix = "";                         // that location
            findWords(english, board, prefix, coordsUsed, row, col, usedWords);
        }
    }
}

/*
 * function: bool findWords(english, board, prefix, coordsUsed, row, col, usedWords)
 * usage: if (findWords(english, board, prefix, coordsUsed, row, col, usedWords)) ...
 * -------------------
 * Recursively finds all words that start with prefix, with the last letter of prefix at board[row][col].
 * If prefix is no longer the beginning of an english word, the tree is pruned.
 * If prefix is a word in the english language, it is added to the computer's score and
 * Set<string> usedWords.
 */
static bool findWords(Lexicon english, Vector<Vector<char> > board, string prefix, Set<string> coordsUsed, int row, int col, Set<string>& usedWords){
    coordsUsed.add(integerToString(row)+integerToString(col));
    prefix += board[row][col];
    if (english.contains(prefix) && prefix.length() >= 4 && !usedWords.contains(prefix)){
        recordWordForPlayer(prefix, COMPUTER);  // if prefix is an english word, it is added to usedWords and the
        usedWords.add(prefix);                  // computers score, but recursion is NOT pruned, as there could be
    }                                           // longer words that start with that word.
    if (!english.containsPrefix(prefix)) return false;  // Recursion is pruned if no english words start with the
    for (int i = -1; i <= 1; i++){              // prefix
        for (int j = -1; j <= 1; j++){
            if (0 <= row+i && row+i < board.size() &&
                    0 <= col+j && col+j < board.size() && !coordsUsed.contains(integerToString(row+i)+integerToString(col+j))){
                (findWords(english, board, prefix, coordsUsed, row+i, col+j, usedWords));
            }
        }
    }
}
