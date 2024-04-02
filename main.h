#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs
#define TIMER_STRING_MAX_LENGTH 20

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

struct Villager {
    int row;
    int col;
    int width;
    int height;
};

struct Platform {
    int row;
    int col;
    int width;
    int height;
};

struct Coin {
    int row;
    int col;
    int width;
    int height;
    int collected;
};

struct House {
    int row;
    int col;
    int width;
    int height;
    int collided;
};

#endif
