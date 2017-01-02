/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim
#include "random.h"   // for randomInteger

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
    }
}

/*
 * function: getNonTermValue(nonTerminal, filename)
 * usage: string nonTerminalValue = getNonTermValue(nonTerminal, filename);
 * -------------------
 * getNonTermValue gets a random possible production for the input nontermial
 * based on the possibilities given in the CFG.
 */
static string getNonTermValue(string nonTerminal, string filename){
    string strNumberPossiblities;
    string line;
    string nonTerminalValue;
    ifstream file(getNormalizedFilename(filename));
    while(true){
        getline(file, line);
        if (line == nonTerminal){ // continue if the line could possibly be the start of
                                  // the definition of the nonterminal
            getline(file, strNumberPossiblities);
            if (isdigit(strNumberPossiblities[0])){
                                  // checks to see if the line actually was the definition
                                  // of a nonterminal (next line will be an int).
                int numberPossibilities = stringToInteger(strNumberPossiblities);
                int randomPossibility = randomInteger(1, numberPossibilities);
                for (int j = 0; j < randomPossibility; j++){ // randomly choose which production
                    getline(file, nonTerminalValue);         // to use.
                }
                return nonTerminalValue;    // return the chosen production
            }
        }
    }
}



/*
 * function: writeStory(string filename)
 * usage: writeStory(filename)
 * -------------------
 * writeStory takes the input of filename and creates a story from
 * the CFG in that file.  string story is set to be the first line
 * of the CFG. Each instance of a nonterminal in the story (which
 * is always <start>) is replaced by a random posssible production
 * (generated in getNonTermValue) for that nonterminal.  This is
 * repeated until there are no more nonterminals in the story.
 */
static void writeStory(string filename){
    string story;
    ifstream file(getNormalizedFilename(filename));
    getline(file, story);
    while(true){
        if (story.find('<') == 4294967295){ // if there are no more nonterminals
            break;                          // in the story, exit the while loop.
        }
        string nonTerminal;
        for (int i=0; i < story.length(); i++){
            if (story[i] == '<'){                           // finds the first noterminal
                for (int j = i; j < story.length(); j++){   // in the story and replaces
                    nonTerminal += story[j];                // it with a possible production
                    if (story[j] == '>'){                   // from getNonTermValue.
                        break;
                    }
                }
                string nonTerminalValue = getNonTermValue(nonTerminal, filename);
                story.erase(i, nonTerminal.length());       // remove the nonterminal
                story.insert(i, nonTerminalValue);          // and replace is with the production
                break;  // restart at the beginning of the story.
            }
        }
    }
    cout << story << endl;
}


int main() {
    setRandomSeed(1);
    while (true) {
        string filename = getFileName();
        if (filename.empty()) break;
        for (int i = 0; i < 3; i++){
            cout << i+1 << ".) ";
            writeStory(filename);
            cout << endl << endl;
        }

    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}
