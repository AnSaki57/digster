#include "treasure.hpp"
#include <vector>

void treasure::initialise(int ty) {
    const static std::vector<int> amount = {0, 25, 150, 1000, 7500, 75000};
    const static std::vector<Color> fillC = {
        {0,0,0,0},
        {240,0,0,255},
        {240,80,0,255},
        {0,240,0,255},
        {0,0,240,255},
        {240,240,0,255}
    }, 
    borderC = {
        {0,0,0,0},
        {180,0,0,255},
        {120,60,0,255},
        {0,180,0,255},
        {0,0,180,255},
        {200,200,20,255}
    };

    type = ty;
    amt = amount[ty];
    fillColour = fillC[ty];
    borderColour = borderC[ty];
}

void treasure::draw() {
    DrawRectangleV(posn, {TREASURE_SIZE, TREASURE_SIZE}, borderColour);
    DrawRectangle(posn.x+TREASURE_BORDER_WIDTH, posn.y+TREASURE_BORDER_WIDTH, TREASURE_SIZE-2*TREASURE_BORDER_WIDTH, TREASURE_SIZE-2*TREASURE_BORDER_WIDTH, fillColour);
}