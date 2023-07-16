#include "field.h"

Field::Field(int value, bool hardCoded) {
    this->value = value;
    this->hardCoded = hardCoded;
    this->cellIndex = -1;
    this->visited = false;
    this->numberOfNeighbours = 0;
    for(int i = 0; i < NEIGHBOURS_SIZE; i++)
        neighbours[i] = -1;
    for(int i = 0; i < CELL_SIZE; i++)
        posibilities[i] = true;
}

void Field::printNeighbours() {
    for(int i = 0; i < NEIGHBOURS_SIZE; i++) {
        if(neighbours[i] == -1)
            break;
        cout << neighbours[i] << ' ';
    }
    cout << '\n';
}

void Field::displayPossibilities() {
    for(int i = 0; i < CELL_SIZE; i++) {
        cout << i + 1 << ": ";
        (this->posibilities[i]) ? cout << "possible" : cout << "not possible";
        cout << endl;
    }
}