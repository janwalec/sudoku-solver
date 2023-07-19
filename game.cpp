#include "game.h"

Game::Game(bool empty) {
    this->numberOfTries = 0;
    this->filledGaps = 0;
    if(!empty) {    //reading board to solve
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
        this->processCells(); 
        this->processNeighbours();
        this->checkPossibilities();
        this->checkByRowsAndCells();
        this->solve();
    } else {    //generating random board
        srand(time(NULL)); 
        for(int i = 0; i < BOARD_SIZE; i++)
            board[i] = new Field(0, false);
        this->processCells(); 
        this->processNeighbours();
        this->createRandomBoard();
    }
   this->drawBoard();
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
    cout << "\n\nTotal number of tries: " << this->numberOfTries << "\n\n";
}

void Game::processCells() {
    int index = 0, currentStart = 0, iter = 0;

    for(int i = 0; i < BOARD_SIZE; i++) {
        if(!i) currentStart = 0;
        if(i == CELL_SIZE * 3) currentStart = 3;
        if(i == CELL_SIZE * 6) currentStart = 6;
        if(i && i % CELL_SIZE == 0) index = currentStart;

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
    for(int i = 0; i < NEIGHBOURS_SIZE; i++) {
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
        
        this->numberOfTries++;
        /*"animation"*/
        drawBoard(); //if you want instant sollution, comment this drawBoard()
    }
    drawBoard();
}

int Game::searchForPossibleEdit(int n) {
    n--;
    while(board[n]->hardCoded)
        n--;    
    return n;
}

void Game::checkPossibilities() {
    int possibleCounter[CELL_SIZE];
    bool doRepeat = false;
    int counter = 0;
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(board[i]->hardCoded) 
            continue;
            
        for(int j = 0; j < CELL_SIZE; j++) {
            board[i]->value = j + 1;
            if(!checkIfFits(i))
                board[i]->posibilities[j] = false;
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
                            if(!checkIfFits(k)) {
                                board[k]->value = 0;
                                continue;
                            }
                            board[k]->hardCoded = true;
                            counter++;
                        }   
                    }
                }
            }
        }
    }
    this->filledGaps += counter;
    if(doRepeat)
        checkPossibilities();   
}

void Game::checkByRowsAndCells() {  
    int possibilitiesInCell[CELL_SIZE];
    int cntr = 0;

    for(int i = 0; i < CELL_SIZE; i++) {
        cntr = 0;

        for(int j = 0; j < CELL_SIZE; j++)
            possibilitiesInCell[j] = 0;

        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[j]->hardCoded)
                continue;

            if(i == board[j]->cellIndex) {
                cntr++;
                for(int k = 0; k < CELL_SIZE; k++) {
                    if(board[j]->posibilities[k])
                        possibilitiesInCell[k]++;
                }
                if(cntr == CELL_SIZE)
                    break;
            }
        }

        for(int j = 0; j < CELL_SIZE; j++) {
            if(possibilitiesInCell[j] == 2) 
                checkIfPossibilitiesAreInOneLine(i, j, 2);
            else if(possibilitiesInCell[j] == 3) 
                checkIfPossibilitiesAreInOneLine(i, j, 3);
        }
    }
}

void Game::checkIfPossibilitiesAreInOneLine(int cellIndex, int whichNumber, int howManyFields) {
    int firstIndex = -1, secondIndex = -1, thirdIndex = -1;
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(board[i]->hardCoded)
            continue;
        if(board[i]->cellIndex == cellIndex) {
            if(board[i]->posibilities[whichNumber]) {
                if(firstIndex < 0)
                    firstIndex = i;
                else if(secondIndex < 0) {
                    secondIndex = i;
                    if(howManyFields == 2)
                        break;
                }
                else if(thirdIndex < 0 && howManyFields == 3) {
                    thirdIndex = i;
                    break;
                }    
            }
        }
    }
    int diffOne, diffTwo;
    if(howManyFields == 2) { //only possible difference between fields can be 1, 2, 9 and 18, other means nothig
        diffOne = secondIndex - firstIndex;
        if(diffOne <= 2)  //same row
            clearRowOrCollumn(true, firstIndex, secondIndex, thirdIndex, whichNumber);   
        else if (diffOne == 9 || diffOne == 18)   //same collumn
            clearRowOrCollumn(false, firstIndex, secondIndex, thirdIndex, whichNumber);
    } else { // first difference must equal second difference
        diffOne = secondIndex - firstIndex;
        diffTwo = thirdIndex - secondIndex;
        if(diffOne == diffTwo) {
            if(diffOne <= 2)
                clearRowOrCollumn(true, firstIndex, secondIndex, thirdIndex, whichNumber);
            else 
                clearRowOrCollumn(false, firstIndex, secondIndex, thirdIndex, whichNumber);
        }
    }
}

void Game::clearRowOrCollumn(bool row, int firstIndex, int secondIndex, int thirdIndex, int whichNumberToClear) {
    int counter = 0;
    int tempIndex = firstIndex;
    if(row) {
        while(tempIndex % 9 != 0)
            tempIndex--;

        for(int i = tempIndex; i < tempIndex + CELL_SIZE; i++) {
            if(board[i]->hardCoded)
                continue;
            if(i != firstIndex && i != secondIndex && i != thirdIndex) {
                if(board[i]->posibilities[whichNumberToClear] == false)
                    continue;
                board[i]->posibilities[whichNumberToClear] = false;
                counter++;
            }
        }
    } else {
        while(tempIndex >= 0)
            tempIndex -= 9;
        tempIndex += 9;
        for(int i = tempIndex; i <= tempIndex + 8 * CELL_SIZE; i += 9) {
            if(board[i]->hardCoded)
                continue;
            if(i != firstIndex && i != secondIndex && i != thirdIndex) {
                if(board[i]->posibilities[whichNumberToClear] == false)
                    continue;
                board   [i]->posibilities[whichNumberToClear] = false;
                counter++;
            }
        }
    }
    if(counter > 1)
        checkPossibilities();
}

void Game::createRandomBoard() {
    int random = rand();
    int i = 0;
    bool doRepeat = false;
    while(i++ < 17) {   
        random = rand();    
        if(board[random % BOARD_SIZE]->hardCoded) {
            i--;
            continue;  
        }
        board[random % BOARD_SIZE]->value = (random % 9) + 1;
        while(!checkIfFits(random % BOARD_SIZE)) {
            board[random % BOARD_SIZE]->value++;
            if(board[random % BOARD_SIZE]->value == 10) {
                doRepeat = true;
                break;
            }
        }
        if(doRepeat) {
            doRepeat = false;
            board[random % BOARD_SIZE]->value = 0;
            i--;
            continue;
        }
        board[random % BOARD_SIZE]->hardCoded = true;
    }
    ofstream myfile;
    myfile.open("generated.txt");
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(i && i % 9 == 0)
            myfile << '\n';
        myfile << board[i]->value;
    }
    this->checkPossibilities();
    this->checkByRowsAndCells();
    if(this->filledGaps < 3) {
        this->filledGaps = 0;
        clearBoard();
        createRandomBoard();
    }
}

void Game::clearBoard() {
    for(int i = 0; i < BOARD_SIZE; i++) {
        board[i]->hardCoded = false;
        board[i]->value = 0;
        for(int j = 0; j < CELL_SIZE; j++)
            board[i]->posibilities[j] = true;
    }
}