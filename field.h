#include <iostream>
#define NEIGHBOURS_SIZE 20
#define CELL_SIZE 9
using namespace std;

class Field {
public:
    bool hardCoded, visited;
    int value, cellIndex, numberOfNeighbours;
    int neighbours [NEIGHBOURS_SIZE];
    bool posibilities[CELL_SIZE];

    Field(int value, bool hardCoded);
    
    //for developing
    void printNeighbours();
    void displayPossibilities();
};