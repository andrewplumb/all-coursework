/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <fstream>   // for ifstream
#include <string>    // for strings
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "random.h"  //for randomChance, randomInteger
#include "gevents.h" // for GMouseEvent
#include "strlib.h" // for string manipulation when reading files

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

static void waitForEnter(string message);
static string welcome();
static int simulationSpeed();
static string selectFile(string preparedFile);
static Grid<int> createGrid(Grid<int>& presentGrid, string file);
static void displayPresentGrid(Grid<int>& presentGrid, LifeDisplay& display);
static void updateFutureGrid(Grid<int>& futureGrid, Grid<int> presentGrid);
static void updateCell(Grid<int>& futureGrid, Grid<int> presentGrid, int i, int j);
static void runSim(Grid<int>& futureGrid, Grid<int>& presentGrid, LifeDisplay& display, int simSpeed);
static void runAgain(Grid<int>& futureGrid, Grid<int>& presentGrid, LifeDisplay& display);

int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");
    int numCols = randomInteger(40, 61);
    int numRows = randomInteger(40, 61);
    Grid<int> presentGrid(numRows, numCols);
    Grid<int> futureGrid(numRows, numCols);
    string file = welcome();
    int simSpeed = simulationSpeed();
    futureGrid = presentGrid = createGrid(presentGrid, file);
    display.setDimensions(presentGrid.numRows(), presentGrid.numCols());
    displayPresentGrid(presentGrid, display);
    updateFutureGrid(futureGrid, presentGrid);
    runSim(futureGrid, presentGrid, display, simSpeed);
    runAgain(futureGrid, presentGrid, display);
    return 0;
}

static void waitForEnter(string message) {
    cout << message;
    (void) getLine();
}

/*
 * function: welcome()
 * usage: string inputFile = welcome();
 * -------------------
 * This function welcoems the user to the game of life, and explains the rules of the game.
 * It then asks the user if he/she would like to use a prepared colony configuration file, or
 * have the program create a random one for him/her.
 */
static string welcome() {
    string preparedFile;
    string inputFile;
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    waitForEnter("Hit [enter] to continue....   ");
    cout << "You can start your colony with random cells or read from a prepared file." << endl;
    while (preparedFile != "yes" && preparedFile != "no"){
        preparedFile = getLine("Do you have a prepared file? Yes or No:");
        for (int i = 0; i < preparedFile.length(); i++){
            preparedFile[i] = tolower(preparedFile[i]);
        }
    }
    inputFile = selectFile(preparedFile);
    return inputFile;
}
/*
 * function: selectFile(preparedFile)
 * usage: string inputFile = selectFile(preparedFile;
 * -------------------
 * Takes the yes or no user input from welcome() and if no, returns random.  If yes,
 * asks user for the file wanted, and checks to see if it exists.  If it exists, returns
 * file name, if not, asks again.
 */

static string selectFile(string preparedFile){
    string inputFile;
    bool fileExists = false;
    if (preparedFile == "no"){
        inputFile = "Random";
    }else if (preparedFile == "yes"){
        while (!fileExists){
            inputFile = getLine("Please enter the file name:");
            ifstream fileName(inputFile);
            if (fileName){
                fileExists = true;
            }else{
                cout << "That file could not be found." << endl;
            }
        }
    }
    return inputFile;
}

/*
 * function: simulationSpeed()
 * usage: int simSpeed = simulationSpeed()
 * -------------------
 * This function tells the user what the options are for simulation speed, and returns the
 * users selection for simulation speed to int simSpeed.
 */

static int simulationSpeed(){
    int simSpeed = 0;
    cout << "How fast would you like to run?:" << endl;
    cout << "1: as fast as possible." << endl;
    cout << "2: wait one quarter of a second between displays." << endl;
    cout << "3: wait one half of a second between displays." << endl;
    while (simSpeed != 1 && simSpeed != 2 && simSpeed != 3 && simSpeed != 4){
        simSpeed = stringToInteger(getLine("4: wait for user to hit enter before next display.  :"));
    }
    cout << "Click in the graphics window to end simulation.  If you selected '4' for speed, you must hit [enter] again." << endl;
    return simSpeed;
}



/*
 * function: createGrid(presentGrid)
 * usage: Grid<int>presentGrid = createGrid(presentGrid);
 * -------------------
 * If the user asked for the program to create a random grid in welcome(), this function fills in a
 * 40 to 60 x 40 to 60 grid (created in main) with 0s and 1s, with a 50% chance of each.
 *
 * If the user entered a valid file name in selectFile(), this function gets each line of that file.
 * It prints any notes to the console, and resizes the grid to the size indicated in the file.
 * It then populates the grid with 0s for - and 1s for x in the file.
 *
 */
static Grid<int> createGrid(Grid<int>& presentGrid, string file){
    if (file == "Random"){
        for (int i=0; i<presentGrid.numRows(); i++){
            for (int j=0; j<presentGrid.numCols(); j++){
                if (randomChance(0.5)){
                    presentGrid[i][j] = 1;
                }else{
                    presentGrid[i][j] = 0;
                }
            }
        }
    } else{
        ifstream fileName(file);
        string fileLine;
        int lineNumber = 0;
        int numRows;
        int numCols;
        while (getline(fileName, fileLine)){
            if (fileLine[0] == '#'){
                cout << fileLine << endl; //Prints any comments from the file to the console
            }else{
                lineNumber++;
                if (lineNumber == 1){
                    numRows = stringToInteger(fileLine);  /*The first line after comments is         */
                                                          /* the number of rows the grid should have */
                }else if (lineNumber == 2){
                    numCols = stringToInteger(fileLine);  /*The second line is the number of rows    */
                    presentGrid.resize(numRows, numCols); /*Resizes presentGrid to the size indicated*/
                                                          /*by the file.                             */
                }else{
                    int rowNumber = lineNumber - 3;         /* The next line in the file is the first row   */
                    for (int j = 0; j < numCols; j++)       /* of the configuration. This is the 3rd line   */
                        if (fileLine[j] == '-'){            /* since we started counting lines (so ignoring */
                            presentGrid[rowNumber][j] = 0;  /* comment lines), so subtracting 3 will give us*/
                        }else if (fileLine[j] == 'X'){      /* the row of presentGrid we are populating.    */
                            presentGrid[rowNumber][j] = 1;  /* The for line loops through the line, and     */
                        }                                   /* populates with a 0 for - and 1 for X.        */
                    }
            }
        }
    }
    return presentGrid;
}

/*
 * function: displayPresentGrid(presentGrid)
 * usage: displayPresentGrid(presentGrid);
 * -------------------
 * uses function drawCellAt from life-graphics.cpp to display the cells
 * in the present grid.
 */

static void displayPresentGrid(Grid<int>& presentGrid, LifeDisplay& display){
    for (int i=0; i<presentGrid.numRows(); i++){
        for (int j=0; j<presentGrid.numCols(); j++){
                display.drawCellAt(i, j, presentGrid[i][j]);
        }
    }
}


/*
 * function: updateFutureGrid(futureGrid, presentGrid)
 * usage: updateFutureGrid(futureGrid, presentGrid);
 * -------------------
 * This function loops through every cell in the grid, and updates the cell in futureGrid
 * by calling updateCell().
 */
static void updateFutureGrid(Grid<int>& futureGrid, Grid<int> presentGrid){
    for (int i=0; i<presentGrid.numRows(); i++){
        for (int j = 0; j<presentGrid.numCols(); j++){
            updateCell(futureGrid, presentGrid, i, j);
        }
    }
}

/*
 * function: updateCell(futureGrid, presentGrid, i, j)
 * usage: updateCell(futureGrid, presentGrid, i, j);
 * -------------------
 * Update cell checks all cells surrounding presentGrid[i][j].  It counts how many neighboring cells are
 * living, and updates futureGrid[i][j] based on that count and the rules explained in welcome().
 */
static void updateCell(Grid<int>& futureGrid, Grid<int> presentGrid, int i, int j){
    int numNeighbors = 0;
    for (int drow = -1; drow <= 1; drow++) {
        for (int dcol = -1; dcol <= 1; dcol++) {                /* Loops through all neighbors of the cell. If   */
            if (presentGrid.inBounds(i+drow, j+dcol)) {         /* a neighbor has value has a cell, increment    */
                if (dcol == 0 && drow == 0){                    /* the numNeighbor counter.  The counter is not  */
                    numNeighbors += 0;                          /* incremented for the cell itself. The value of */
                }else if (presentGrid[i+drow][j+dcol] != 0){    /* the counter after these loops is the number of*/
                    numNeighbors += 1;                          /* neighbors.                                    */
                }
            }
        }
    }
    if (numNeighbors == 2){
        if (futureGrid[i][j] > 0 && futureGrid[i][j] < kMaxAge){
            futureGrid[i][j] += 1;
        }
    }else if (numNeighbors == 3){
        if (futureGrid[i][j] < kMaxAge){
            futureGrid[i][j] += 1;
        }
    } else{
        futureGrid[i][j] = 0;
    }
}


/*
 * function: runSim(futureGrid, presentGrid, display)
 * usage: runSim(futureGrid, presentGrid, display);
 * -------------------
 * This function runs the simulation. It starts by checking if the mouse has been clicked.  If it has, it ends the simulation.
 * If not, it checks to see if futureBoard and presentBoard are the same.  If they are, the simulation has stablized and it ends.
 * If not, it waits based on user input in simulationSpeed(), and copys futureGrid into presentGrid.  presentGrid is displayed,
 * and futureGrid is updated again.  This loop then continues until the mouse is clicked, or presentSim and futureSim are the same
 * after updateFutureGrid is run.
 */
static void runSim(Grid<int>& futureGrid, Grid<int>& presentGrid, LifeDisplay& display, int simSpeed){
    while (true) {
        GMouseEvent me = getNextEvent(MOUSE_EVENT);\
        if (me.getEventType() == MOUSE_CLICKED) {
            return;
        } else if (me.getEventType() == NULL_EVENT) {
            // only advance board if there aren’t any outstanding mouse events
            //advanceBoard(futureGrid, presentGrid, display);
            if (futureGrid.toString() == presentGrid.toString()){
                return;
            }else {
                if (simSpeed == 2){
                    pause(250);
                }else if(simSpeed == 3){
                    pause(500);
                }else if(simSpeed == 4){
                    waitForEnter("Hit [enter] to display next generation.");
                }
                presentGrid = futureGrid;
                displayPresentGrid(presentGrid, display);
                updateFutureGrid(futureGrid, presentGrid);
            }
        }
    }
}

/*
 * function: runAgain(futureGrid, presentGrid, display)
 * usage: runAgain(futureGrid, presentGrid, display);
 * -------------------
 * After the simulation ends, runAgain asks the user if they would like to run another simulation.  If the user answers yes,
 * the simulation is rerun.  If the user answers no, the program exits after the user hits enter.
 *
 */
static void runAgain(Grid<int>& futureGrid, Grid<int>& presentGrid, LifeDisplay& display){
    cout << "The simulation has ended." << endl;
    string rerun;
    while (rerun != "yes" && rerun != "no"){
        rerun = getLine("Would you like to run another simulation? Yes or No:");
        for (int i = 0; i < rerun.length(); i++){
            rerun[i] = tolower(rerun[i]);
        }
    }
    if (rerun == "yes"){
        string preparedFile;
        string inputFile;
        cout << "You can start your colony with random cells or read from a prepared file." << endl;
        while (preparedFile != "yes" && preparedFile != "no"){
            preparedFile = getLine("Do you have a prepared file? Yes or No:");
            for (int i = 0; i < preparedFile.length(); i++){
                preparedFile[i] = tolower(preparedFile[i]);
            }
        }
        inputFile = selectFile(preparedFile);
        string file = inputFile;
        int simSpeed = simulationSpeed();
        futureGrid = presentGrid = createGrid(presentGrid, file);
        display.setDimensions(presentGrid.numRows(), presentGrid.numCols());
        displayPresentGrid(presentGrid, display);
        updateFutureGrid(futureGrid, presentGrid);
        runSim(futureGrid, presentGrid, display, simSpeed);
        runAgain(futureGrid, presentGrid, display);
    }else{
        waitForEnter("Hit [enter] to exit....   ");
    }
}
