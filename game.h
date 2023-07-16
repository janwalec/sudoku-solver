#include "field.h"
#include <ctime>
#define BOARD_SIZE 81

class Game {
    void processCells(); //one cell is a 3x3 matrix
    void processNeighbours(); //save every field's neighbour's indexes in its array of neighbours
    bool checkIfFits(int n);
    void checkPossibilities();
    int bestResult;

public:
    Field* board[BOARD_SIZE];
    void clearIsVisited(int n);
    Game();
    ~Game();
    void drawBoard();
    void solve();
    int searchForPossibleEdit(int n);
    void a();
};