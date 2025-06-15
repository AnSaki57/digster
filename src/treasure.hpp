#include "raylib.h"

#define TREASURE_SIZE 30
#define TREASURE_BORDER_WIDTH 2

class treasure {
public:
    Vector2 posn;   // *Not* posn of spot on which it is on, but the posn of the spot that manually sets this (change this)
    int type;

    int amt;
    Color fillColour;
    Color borderColour;

    void initialise(int ty);
    void draw();
};