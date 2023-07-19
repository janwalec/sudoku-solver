#include <iostream>
#define NEIGHBOURS_SIZE 20
#define CELL_SIZE 9
using namespace std;

class Field {
public:
    bool hardCoded, visited;    /*hardCoded ? field can't be changed : field can be changed 
    visited flag is used for creating neighbours array*/  
    int value, cellIndex, numberOfNeighbours; /*value - current value in one    cellIndex - index of cell that field is located in
    numberOfNeighbours is used in iteration while generating neighbours, it should be 20 as pre-defined*/
    int neighbours [NEIGHBOURS_SIZE]; //array of indexes of neighbours
    bool posibilities[CELL_SIZE];   //table of truth for possible fills. default all are true, if some number cant be fitted - false
    Field(int value, bool hardCoded);
    
    //for developing
    void printNeighbours();
    void displayPossibilities();
};