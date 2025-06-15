#include "raylib.h"
#include "treasure.hpp"
#include "shader.hpp"

#define SPOT_SIZE 60
#define SPOT_BORDER_THICKNESS 1
#define DARK_GREY {100,100,100,255}
#define DARKER_GREY {45,45,45,255}
#define INFO_FONT_SIZE 18
#define DEBUG_INFO_FONT_SIZE 11
#define INFO_FONT_BUFFER 3
#define HARDNESS_MULTIPLICATION_FACTOR 1.04

// Format a double to 1 decimal place
static auto doubleFormat(double value);

class spot {
public:
    Vector2 posn;
    int depth;
    double currentHealth;
    double totalHealth;
    double damageShielding; // Currently unused
    int cursorState;    // 0 - default; 1 - hover; 2 - click

    Texture2D texture;
    treasure treas;
    shader shade;

    void initialise(Vector2 pos, Texture2D tex);
    int mine(double strike);
    void draw(Font textFont);
    void debugShow();
    void close();
};