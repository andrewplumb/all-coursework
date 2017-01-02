/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "vector.h"
#include "queue.h"

static string getWord(Lexicon& english, string prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

/*
 * function: generateLadder(english, start, end)
 * usage: generateLadder(english, start, end);
 * -------------------
 * This function finds the shortest word ladder between the start and end words
 * using a breadth-first search. The head vector will be dequeued from a queue of possible
 * word ladders.  If the last word of this ladder matches the end word, then that ladder is the shortest
 * correct word ladder linking the words.  Otherwise, every potential word that is 1 letter different from
 * the top word will be checked to see if it is in the english dictionary and hasn't been used in a
 * previous ladder.  If this is the case, it is added to the end of the ladder, and the new ladder is
 * enqueued. Once all possible words have been checked, and existing not-yet-used words added to the ladder
 * and enqueued, the next ladder is dequeued and the process repeats.  This ensures the shortest ladder will
 * be found.
 */
static void generateLadder(Lexicon& english, string start, string end) {
    if (start.length() != end.length()){
        cout << "The start and end word must contain the same number of letters, else no word " <<
                "ladder can exist." << endl;
        return;
    }
    Queue<Vector<string>> queue;
    Vector<string> ladder;
    Vector<char> alphabet;
    alphabet += 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                 'u', 'v', 'w', 'x', 'y', 'z';
    Lexicon usedWords;
    usedWords.add(start); // add the starting word to usedWords (don't want to come back to it!)
    ladder.add(start);
    queue.enqueue(ladder);
    while (!queue.isEmpty()){
        ladder = queue.dequeue();
        if (ladder[ladder.size()-1] == end) {  // Check to see if the last word of the ladder
            cout << "Found Ladder: ";          // is the target word.
            for (int i = 0; i < ladder.size(); i++){
                if (i > 0) cout << ", ";
                cout << ladder[i];
            }
            cout << endl;
            return;
        } else{
            string topWord = ladder[ladder.size()-1]; // pull the top word off the ladder.
            string tempTopWord;
            for (int i = 0; i < topWord.size(); i++){ // loop through changing each letter of the word
                for (int j = 0; j < alphabet.size(); j++){ // to each letter of the alphabet.  If the
                    tempTopWord = topWord;                 // if the word created is in the english
                    tempTopWord[i] = alphabet[j];          // dictionary, it is added to a ladder and
                                                           // enqueued.
                    if (english.contains(tempTopWord) && !usedWords.contains(tempTopWord)){
                        ladder.add(tempTopWord);
                        usedWords.add(tempTopWord);
                        queue.enqueue(ladder);
                        ladder.remove(ladder.size()-1);
                    }
                }
            }
        }
    }
    cout << "No word ladder between " << start << " and " << end << " could be found." << endl;
}

static const string kEnglishLanguageDatafile = "EnglishWords.dat";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
