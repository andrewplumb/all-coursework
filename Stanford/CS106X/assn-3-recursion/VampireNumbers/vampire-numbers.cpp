/**
 * File: vampire-numbers.cpp
 * -------------------------
 * Provides a program that repeatedly prompts the user for positive
 * numbers with an even number of digits and informs them
 * whether the number is a Vampire number or not.
 */

#include <string>    // for string
#include <iostream>  // for cout, endl
using namespace std;

#include "console.h" 
#include "simpio.h" // for getInteger
#include "strlib.h" // for stringToInteger, integerToString
#include "set.h"

static Set<string> generatePermutations(string str, int number);

static int getPossibleVampireNumber() {
    while (true) {
        int response = getInteger("Enter a positive integer [or 0 to quit]: ");
        if (response >= 0) return response;
        cout << "Ah, sorry.  I need a nonnegative response.  Try again... " << endl;
    }
}

/*
 * function: isVampireNumber(int number, int& first, int& second)
 * usage: isVampireNumber(number, first, second)
 * -------------------
 * Returns TRUE if number is a vampire number, and sets first and second to its
 * fangs.  Otherwise it returns FALSE.  Unique permutations of number are generated
 * and stored in a set until a match is found.  These permutations are looped through,
 * and first is set to the first half of each permutation and second is set to the second
 * half.  If the first half times the second half equals the number, then first and second
 * are the fangs and number is a vampire number.
 *
 * I thought of what I think would be a more efficient algorithm, but I did not have time
 * to implement it.  The fangs are built from the ones digit up, and only combinations of
 * ones digits such that the ones digit of first multiplied by the ones digit of second
 * will give a result that's ones digit is equal to the ones digit of number.  If the ones
 * digit of first and second multiply to something else, then first and second obviously
 * could not be fangs.  Possible values for the ones digits in the fangs are then built
 * upon with possible values for the 10s digit using the same logic.  The last two digits of
 * the fangs must multiply to the last two digits of the number.
 * An example: 1827 is a vampire number.  The last digits of the fangs must be 1 and 7, as
 * 1*8 != x7, 1*2 != x7, 8*2 != x7, and 8*7 != x7.  Possibilities for the 10s digits are then
 * 8 and 2.  27*81 = xx87, not xx27, so that is thrown out.  That leaves 87*21 = xx27.  All
 * digits of the parent number have been used, and we have one posibility.  87*21 = 1827, our
 * answer.
 */
static bool isVampireNumber(int number, int& first, int& second) {
    string numberString = integerToString(number);
    Set<string> permutations = generatePermutations(numberString, number);
    foreach(string str in permutations){
        first = stringToInteger(str.substr(0, str.length()/2));
        second = stringToInteger(str.substr(str.length()/2));
        if (first * second == number) return true;
    }

    return false;
}
/*
 * function: generatePermutations(string str, int number)
 * usage: Set<string> result = generatePermutations(str, number)
 * -------------------
 * Returns a set of all permutations of input string.  This algorithm is taken
 * directly from the textbook, Programming Abstractions in C++ by Eric S. Roberts,
 * page 366, with minor modifications.  If the first half of the permutation created
 * multiplied by the second half of the permutation created is the number, then it
 * stops generating more permutations.
 */
static Set<string> generatePermutations(string str, int number){
    Set<string> result;
    if (str == ""){
        result += "";
    }else{
        for (int i = 0; i < str.length(); i++){
            char ch = str[i];
            string rest = str.substr(0, i) + str.substr(i+1);
            for (string s : generatePermutations(rest, number)){
                string currentString = ch + s;
                result += currentString;
                if (currentString.size() == integerToString(number).size()){
                    int first = stringToInteger(currentString.substr(0, currentString.length()/2));
                    int second = stringToInteger(currentString.substr(currentString.length()/2));
                    if (first * second == number) return result;
                }
            }
        }
    }
    return result;
}

int main() {
    cout << "Here's a program that tells you whether or not a "
         << "number you enter is Vampire." << endl << endl;
    while (true) {
        int number = getPossibleVampireNumber();
        if (number == 0) break;
        if (number < 10) cout << "Nope! The number " << number << " isn't Vampire." << endl << endl;
        else{
            int first, second;
            if (integerToString(number).length()%2 == 1){ // warns the user that they entered a number with an odd
                string tryAnyway;                         // number of digits, and this program is not guaranteed
                                                          // to work.
                while (tryAnyway != "yes" && tryAnyway != "no"){
                    tryAnyway = getLine("This is only guaranteed to work if the number has an even number of digits.  Try anyway? Yes or No:");
                    for (int i = 0; i < tryAnyway.length(); i++){
                        tryAnyway[i] = tolower(tryAnyway[i]);
                    }
                }
                if (tryAnyway == "no"){
                continue;
                }
            }
            if (isVampireNumber(number, first, second)) {
                cout << "Woo! " << number << " is a Vampire number, and "
                 << first << " and " << second << " are its fangs." << endl << endl;
            } else {
                cout << "Nope! The number " << number << " isn't Vampire." << endl << endl;
            }
        }
    }
    cout << endl;
    cout << "All done!" << endl;
    return 0;
}
