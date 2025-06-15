#pragma once

#include "raylib.h"
#include <iostream>

#define FONT_SIZE_FADEOUT_SMALL 18
#define FONT_SIZE_FADEOUT_MEDIUM 27
#define FONT_SIZE_FADEOUT_LARGE 38
#define FONT_SIZE_FADEOUT_GIGANTIC 50

class fadeout {
public:
    std::string msg;
    Color colour;
    int fontSize;
    int currFrame;
    int frameCount;
    Vector2 posn;
    Vector2 vel;

    void initialise(std::string message, Color c, Vector2 position, Vector2 velocity, int frames, int fsize);
    void run();
    void draw();
};