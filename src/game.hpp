#include <vector>
#include "raylib.h"
#include "spots.hpp"
#include "fadeout.hpp"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT WINDOW_WIDTH
#define FONT_SIZE_HEADING 50
#define SPOT_ROWS 10
#define SPOT_COLS SPOT_ROWS
#define GRID_BEGIN_X 50
#define GRID_BEGIN_Y 350
#define AMOUNT_FONT_SIZE 50
#define BUFFER_GRID_AMOUNT 15
#define SPOT_TEXTURES_COUNT 9
#define RIGHT_BOX_WIDTH 250

class game{
public:
    int amt;
    std::vector<int> resources;
    int mineStrength;

    int frame;
    Texture2D background;
    std::vector<spot> spots;
    std::vector<fadeout> fadeouts;
    std::vector<Texture2D> spotTextures;
    std::vector<Texture2D> cursorTextures;
    Font defaultFont, titleFont;

    void getShadow(int i);
//public:
    void initialise();
    void handleEvents();
    void frameProcess();
    void draw();
    void close();
};