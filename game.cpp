#include "game.h"

Game::Game() {
    int v, i = 0;
    char c;  
    while(c = getchar()) {
        if(c == '\xff' || i == BOARD_SIZE)
            break;
        if(c <= ' ')
            continue;
        v = c - '0';
        !v ? board[i] = new Field(0, false) : board[i] = new Field(v, true);
        i++;
    }
    this->bestResult = 0;
    this->processCells(); 
    this->processNeighbours();
    this->checkPossibilities();
}

Game::~Game() {
    for(int i = 0; i < BOARD_SIZE; i++)
        delete board[i];
}

void Game::drawBoard()   {
    int curr;
    system("clear");
    
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(i % CELL_SIZE == 3 || i % CELL_SIZE == 6)    //middle spaces
            cout << "  ";
        if(i % CELL_SIZE == 0)  //enters
            cout << '\n';
        if(i && i % (CELL_SIZE * 3) == 0)   //middle enters
            cout << '\n';

        (curr = board[i]->value) ? cout << curr : cout << '.';  //values interpretation
        cout << ' ';  
    }
    cout << "\n\nCurrent best fit: " << this->bestResult << " / 81 fields\n";

}

void Game::processCells() {
    int index = 0, currentStart = 0, iter = 0;

    for(int i = 0; i < BOARD_SIZE; i++) {
        if(!i) currentStart = 0;
        if(i == CELL_SIZE * 3) currentStart = 3;
        if(i == CELL_SIZE * 6) currentStart = 6;
        if(i && i % CELL_SIZE == 0) 
            index = currentStart;

        board[i]->cellIndex = index;
        iter++;

        if(iter && iter % 3 == 0) {
            iter = 0;
            index++;
        }
    }
}

void Game::processNeighbours() {
    int currentCellIndex;
    for(int i = 0; i < BOARD_SIZE; i++) {
        currentCellIndex = board[i]->cellIndex;
        board[i]->visited = true;

        for(int j = 0; j < BOARD_SIZE; j++) { //check cells
            if(board[j]->cellIndex == currentCellIndex && !(board[j]->visited)){
                board[i]->neighbours[board[i]->numberOfNeighbours++] = j;
                board[j]->visited = true;
                if(board[i]->numberOfNeighbours == CELL_SIZE - 1)
                    break;
            }
        }
        int x = i;
        while((x -= 9) >= 0) { //uppers
            if(board[x]->visited)
                continue;
            board[x]->visited = true;
            board[i]->neighbours[board[i]->numberOfNeighbours++] = x;
        }

        x = i;
        while((x += 9) <= BOARD_SIZE - 1) { //down
            if(board[x]->visited)
                continue;
            board[x]->visited = true;
            board[i]->neighbours[board[i]->numberOfNeighbours++] = x;
        } 

        x = i;
        x = x - (x % 9);
        for(int j = x; j < x + 9; j++) { //from left to right
            if(board[j]->visited)
                continue;
            board[j]->visited = true;
            board[i]->neighbours[board[i]->numberOfNeighbours++] = j;
        }
        clearIsVisited(i);
    }
}

void Game::clearIsVisited(int n) {
    board[n]->visited = false;
    for(int i = 0; i < NEIGHBOURS_SIZE; i++) {
        if(board[n]->neighbours[i] == -1)
            return;
        board[board[n]->neighbours[i]]->visited = false;        
    }
}

bool Game::checkIfFits(int n) {
    for(int i = 0; i < NEIGHBOURS_SIZE; i++){
        if(board[n]->value == board[board[n]->neighbours[i]]->value)
            return false;
    }
    return true;
}

void Game::solve() {
    int i = 0;
    while(i < BOARD_SIZE) {
        if(board[i]->hardCoded) {
            i++;
            continue;   
        }
        board[i]->value++;
        while(!board[i]->posibilities[board[i]->value - 1]) {
            if(board[i]->value++ == 10)
                break;
        }
    
        if(board[i]->value >= 10) {
            board[i]->value = 0;
            i = searchForPossibleEdit(i);
            continue;
        }
        if(checkIfFits(i))
            i++;
        if(i > this->bestResult)
            this->bestResult = i;
       
        drawBoard();
    }
}

int Game::searchForPossibleEdit(int n) {
    n--;
    while(board[n]->hardCoded) {
        n--;
    }
    return n;
}

void Game::checkPossibilities() {
    int possibleCounter[CELL_SIZE];
    bool doRepeat = false;
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(board[i]->hardCoded) 
            continue;
            
        for(int j = 0; j < CELL_SIZE; j++) {
            board[i]->value = j + 1;
            if(!checkIfFits(i)) {
                board[i]->posibilities[j] = false;
            }
        }
        board[i]->value = 0;
    }

    for(int i = 0; i < CELL_SIZE; i++) { //go through every single cell, from 0-8
        for(int j = 0; j < CELL_SIZE; j++) // reset at the begining
            possibleCounter[j] = 0;
        
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[j]->hardCoded)
                continue;
            if(board[j]->cellIndex == i) { //if it is this cell - go into
                for(int k = 0; k < CELL_SIZE; k++) { //go through all true/false possibilities
                    if(board[j]->posibilities[k])
                        possibleCounter[k]++; //if it fits, add this to counter
                }
            }
        }
        for(int j = 0; j < CELL_SIZE; j++) {
            if(possibleCounter[j] == 1) {   //if counter is 1 there is only one place to fit this number
                doRepeat = true;
                for(int k = 0; k < BOARD_SIZE; k++) { //search for cell once again
                    if(board[k]->hardCoded) //skip coded fields
                        continue;
                    if(board[k]->cellIndex == i) { 
                        if(board[k]->posibilities[j]) {
                            board[k]->value = j + 1;
                            board[k]->hardCoded = true;
                        }
                    }
                }
            }
        }
    }
    if(doRepeat)
        checkPossibilities();
}

void Game::a() {
    cout << "a";
    int random = 0;
    int currentIndex, value;
    bool repeat = false;
    for(int i = 0; i < BOARD_SIZE; i++){
        delete board[i];
        board[i] = new Field(0, false);
    }

    for(int i = 0; i < 17; i++) {
        random += (i + 7) * 11;
        random = random / 2;
        random = random + rand();
        currentIndex = random % 81;
        if(board[currentIndex]->hardCoded) {
            i--;
            continue;
        }
        value = random % 10;
        if(!repeat) {
            board[currentIndex]->value = value;
            if(checkIfFits(currentIndex)) {
                repeat = true;
                i--;
                continue;
            }
            repeat = false;
            board[currentIndex]->hardCoded = true;
        }

    }
}