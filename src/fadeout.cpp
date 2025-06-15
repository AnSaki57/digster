#include "fadeout.hpp"
#include "raylib.h"
#include <iostream>

// Initialise parameters
void fadeout::initialise(std::string message, Color c, Vector2 position, Vector2 velocity, int frames, int fsize) {
    msg = message;
    colour = c;
    posn = position;
    vel = velocity;
    currFrame = 0;
    frameCount = frames;
    fontSize = fsize;
}

// Increment internal frame count
void fadeout::run() {currFrame++;}

// Draw the fadeout based on the state of the fadeout
void fadeout::draw() {
    double opacity = 255 * (1 - (double)currFrame/frameCount);
    if (opacity<0) opacity=0;

    DrawText(msg.c_str(), posn.x+vel.x*currFrame, posn.y+vel.y*currFrame, fontSize, {colour.r, colour.g, colour.b, (unsigned char)(int)opacity});
}