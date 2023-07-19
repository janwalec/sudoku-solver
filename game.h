#include "field.h"
#include <ctime>
#include <fstream>
#define BOARD_SIZE 81

class Game {
    Field* board[BOARD_SIZE];
    int numberOfTries, filledGaps; 
    /*number of tries - displaying how many times solve() was caled
    filled gaps - counts how many fields can be filled with checkPossibilities(), mainly used for generating*/
    void processCells(); //one cell is a 3x3 matrix
    void processNeighbours(); //save every field's neighbour's indexes in its array of neighbours
    bool checkIfFits(int n); //check if value under n-index is legal
    void checkPossibilities(); //check if there is a field in a cell that got only 1 possibility, fill that field
    
    void checkByRowsAndCells(); /*check if in one cell there are 2 or 3 squares with the same number possible, they might will
    create a line and delete possibilities in neighbouring cells, 
    see the example: (grid of possibilities in sudoku, 'n' - possible n number to fit)
    nn. ... ...     'n' are only possible in first cell in two marked fields, this means whole row number one is cleared from n  
    ... ... ...     checkIfPossibilitiesAreInOneLine(...) checks if 'n' are lined up
    ... ... ... */ 
    void checkIfPossibilitiesAreInOneLine(int cellIndex, int whichNumber, int howManyFields); 
    void clearRowOrCollumn(bool row, int firstIndex, int secondIndex, int thirdIndex, int whichNumberToClear);
    
    int searchForPossibleEdit(int n); //used with solve(), checks first possible field to edit behind n-indexed field
    void clearIsVisited(int n); //used with processNeighbours(), clears 'bool visited' from fields
    void createRandomBoard();   //generate random board, may generate sudoku with more than one sollution
    void clearBoard();  //used with createRandomBoard(), if sudoku is not solvable, it clears the board
    void solve(); //solves sudoku with brute force, usedd if previous algorithms can't do that (mainly for hard patterns)

public:
    Game(bool empty); //constructor, empty ? generate random board : get board from input and solve it
    ~Game();
    void drawBoard(); //simply draws the board
};