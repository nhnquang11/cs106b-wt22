#pragma once
#include <string>

struct Cell {
    int value;
    Cell *next;
};

Cell* concat(Cell* one, Cell* two);
