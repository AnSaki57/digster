#include "spots.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

// (Helper function) Format a double to 1 decimal place
static auto doubleFormat(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << value;
    return oss.str();
};

void spot::initialise(Vector2 pos, Texture2D tex) {
    srand(time(0));

    posn = pos;
    texture = tex;
    depth = 0;
    totalHealth = currentHealth = 100;
    
    treas.initialise(0);
    treas.posn = {pos.x+(SPOT_SIZE-TREASURE_SIZE)/2, pos.y+(SPOT_SIZE-TREASURE_SIZE)/2};
    shade.initialise(pos, SPOT_SIZE);
}

int spot::mine(double strike) {
    if (treas.type!=0) {
        int res = treas.amt;
        treas.initialise(0);
        return res;
    } 
    
    currentHealth-=strike;
    if (currentHealth<=0) {
        depth++;
        totalHealth*=HARDNESS_MULTIPLICATION_FACTOR;
        currentHealth=totalHealth;

        // chance of treasure
        int tchance = rand() % 6250;
        if (tchance<=780) {
            if (tchance>155) {if (depth>5) treas.initialise(1);}
            else if (tchance>30) {if (depth>15) treas.initialise(2);}
            else if (tchance>5) {if (depth>50) treas.initialise(3);}
            else if (tchance>0) {if (depth>125) treas.initialise(4);}
            else if (depth>500) treas.initialise(5);
        }

        return 10;
    }

    return 0;
}

void spot::draw(Font textFont) {
    Color borderColour = DARKER_GREY;
    int borderThickness = SPOT_BORDER_THICKNESS;

    if (cursorState > 0) {  // If cursor hovers over the spot
        borderColour = {255,0,255,255};
        borderThickness=SPOT_BORDER_THICKNESS*2;
    } else borderThickness=SPOT_BORDER_THICKNESS;

    DrawTexture(texture, posn.x+SPOT_BORDER_THICKNESS, posn.y+SPOT_BORDER_THICKNESS, WHITE);
    /*if (!(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && cursorState > 0))*/ shade.draw();
    DrawRectangleLinesEx({posn.x, posn.y, SPOT_SIZE, SPOT_SIZE}, borderThickness, borderColour);    // Draw border (may be purple, on cursor hover)

    DrawTextEx(textFont, (std::to_string(depth)).c_str(), {posn.x+SPOT_BORDER_THICKNESS+INFO_FONT_BUFFER, posn.y+SPOT_BORDER_THICKNESS+INFO_FONT_BUFFER}, INFO_FONT_SIZE, 0, {255,255,255,255});

    treas.draw();
}

void spot::debugShow() {
    DrawText((doubleFormat(currentHealth) + "," + doubleFormat(totalHealth)).c_str(), posn.x, posn.y+SPOT_SIZE/2, DEBUG_INFO_FONT_SIZE, BLACK);
    DrawText(std::to_string(treas.type).c_str(), posn.x, posn.y+SPOT_SIZE/2+DEBUG_INFO_FONT_SIZE+INFO_FONT_BUFFER, DEBUG_INFO_FONT_SIZE, RED);
}

void spot::close() {
    UnloadTexture(texture);
}