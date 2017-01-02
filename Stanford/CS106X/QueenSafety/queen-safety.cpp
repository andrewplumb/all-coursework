#include <iostream>
#include <string>
using namespace std;

#include "simpio.h"
#include "random.h"
#include "gwindow.h"
#include "gobjects.h"
#include "grid.h"

static const int kNumQueens	= 8;
static const int kBoardDimension = 8;
static const int kNumRows = kBoardDimension;
static const int kNumColumns = kBoardDimension;
static const double kSquareSize = 32;
static const double kMillisecondDelay = 250;

static void drawSquare(GWindow& window, int row, int col, string color) {
    int ulx = (window.getWidth() - kBoardDimension * kSquareSize)/2 + col * kSquareSize;
    int uly = (window.getHeight() - kBoardDimension * kSquareSize)/2 + row * kSquareSize;
    window.setColor(color);
    window.drawRect(ulx, uly, kSquareSize, kSquareSize);
}

static void clearBoard(Grid<bool>& board, GWindow& window) {
	for (int row = 0; row < board.numRows(); row++) {
		for (int col = 0; col < board.numCols(); col++) {
			board[row][col] = false;
            drawSquare(window, row, col, "Blue"); // implementation omitted
		}
	}
}

static void markLocation(GWindow& window, string label, int row, int col, string color) {
    int cx = (window.getWidth() - kBoardDimension * kSquareSize)/2 + col * kSquareSize + kSquareSize/2;
    int cy = (window.getHeight() - kBoardDimension * kSquareSize)/2 + row * kSquareSize + kSquareSize/2;
    window.setColor(color);
    GLabel *lab = new GLabel(label);
    lab->setFont("Helvetica-Bold-24");
    window.add(lab, cx - lab->getWidth()/2, cy + lab->getHeight()/2);
    pause(kMillisecondDelay);
}

static void placeRandomQueens(Grid<bool>& board, GWindow& window) {
	int numQueensPlaced = 0;
	while (numQueensPlaced < kNumQueens) {
		int row = randomInteger(0, board.numRows() - 1);
		int col = randomInteger(0, board.numCols() - 1);
		if (!board[row][col]) {
			board[row][col] = true;
			markLocation(window, "Q", row, col, "Black");
			numQueensPlaced++;
		}
	}
	
	cout << "Click the mouse anywhere in the window to see which locations are safe." << endl;
    waitForClick();
}

static bool isDirectionSafe(Grid<bool>& board, int row, int col, int drow, int dcol) {
	if (drow == 0 && dcol == 0) {
        return true;
    }
    
    row += drow;
	col += dcol;
	while (board.inBounds(row, col) && !board[row][col]) {
		row += drow;
		col += dcol;
	}
    
	return !board.inBounds(row, col);
}

static bool isSafe(Grid<bool>& board, int row, int col) {
	for (int drow  = -1; drow  <= 1; drow++) {
		for (int dcol = -1; dcol <= 1; dcol++) {
			if (!isDirectionSafe(board, row, col, drow, dcol))
				return false;
		}
	}
	
	return true;
}

static void identifySafeLocations(Grid<bool>& board, GWindow& window) {
	for (int row = 0; row < board.numRows(); row++) {
		for (int col = 0; col < board.numCols(); col++) {
			if (!board[row][col]) {
				if (isSafe(board, row, col)) {
					markLocation(window, "S", row, col, "Green");
				} else {
					markLocation(window, "X", row, col, "Red");
				}
			}
		}
	}
}
int main() {
	Grid<bool> board(kNumRows, kNumColumns);
    GWindow window;
    window.setWindowTitle("Queen Safety");
    window.setVisible(true);
	clearBoard(board, window);
	placeRandomQueens(board, window);
	identifySafeLocations(board, window);
    return 0;
}
