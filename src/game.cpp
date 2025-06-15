#include <random>
#include "game.hpp"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

void game::initialise() {
    // Initialise Raylib window, set configs
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SearchAndSetResourceDir("resources");
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Digster Ver 1.0");

    // RNG mechanism
    std::random_device rd;                          // seeds from system entropy
    std::mt19937 gen(rd());                         // Mersenne Twister RNG
    std::uniform_int_distribution<int> dist(0, SPOT_TEXTURES_COUNT-1);

    // Initialise non-vector attributes
    amt = 0;
    resources.resize(5, 0);
    frame = 0;
    mineStrength = 50;
    fadeouts.resize(0);
    spotTextures.resize(SPOT_TEXTURES_COUNT);
    cursorTextures.resize(3);

    // Load background
    Image bg = LoadImage("Background3.png");
    ImageCrop(&bg, {256, 0, 1024, 1024});
    ImageResize(&bg, 1400, 1340);
    background = LoadTextureFromImage(bg);
    
    // Load soil textures (all 9)
    Image spotTextureSprites = LoadImage("EarthSprites4.png");
            
    Image currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {50, 322, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[0] = LoadTextureFromImage(currSprite);

    currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {380, 322, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[1] = LoadTextureFromImage(currSprite);

    currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {700, 322, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[2] = LoadTextureFromImage(currSprite);

    currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {50, 650, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[3] = LoadTextureFromImage(currSprite);

    currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {380, 650, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[4] = LoadTextureFromImage(currSprite);

    currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {700, 650, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[5] = LoadTextureFromImage(currSprite);

    currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {50, 960, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[6] = LoadTextureFromImage(currSprite);

    currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {380, 960, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[7] = LoadTextureFromImage(currSprite);

    currSprite = ImageCopy(spotTextureSprites);
    ImageCrop(&currSprite, {700, 960, 256, 256});
    ImageResize(&currSprite, SPOT_SIZE-2*SPOT_BORDER_THICKNESS, SPOT_SIZE-2*SPOT_BORDER_THICKNESS);
    spotTextures[8] = LoadTextureFromImage(currSprite);

    
    // Load cursor textures (plain cursor, pickaxe upright and striking)
    HideCursor();
    currSprite = LoadImage("Sprites.png");   
    ImageCrop(&currSprite, {0,0,512,512});
    ImageResize(&currSprite, 32, 32);
    cursorTextures[0] = LoadTextureFromImage(currSprite);

    currSprite = LoadImage("PickaxeUpright.png");   
    ImageCrop(&currSprite, {90,180,800,1200});
    ImageResize(&currSprite, 64, 96);
    cursorTextures[1] = LoadTextureFromImage(currSprite);

    currSprite = LoadImage("PickaxeStriking.png");   
    ImageResize(&currSprite, 64, 96);
    cursorTextures[2] = LoadTextureFromImage(currSprite);

    UnloadImage(currSprite);
    UnloadImage(spotTextureSprites);

    // Initialise the spots (with random textures)
    spots.resize(SPOT_ROWS*SPOT_COLS);

    for (int i = 0; i < SPOT_ROWS; i++) {
        for (int j = 0; j < SPOT_COLS; j++) {
            spots[i*SPOT_ROWS+j].initialise({(float)GRID_BEGIN_X+i*SPOT_SIZE,(float)GRID_BEGIN_Y+j*SPOT_SIZE}, spotTextures[dist(gen)]);
        }
        std::cout << "\n";
    }

    // Set the default fonts for stuff
    defaultFont = LoadFontEx("Roboto/static/Roboto_Condensed-Regular.ttf", 50, 0, 0);
    SetTextureFilter(defaultFont.texture, TEXTURE_FILTER_BILINEAR);
    titleFont = LoadFontEx("Roboto/static/Roboto_SemiCondensed-Bold.ttf", 150, 0, 0);
    SetTextureFilter(titleFont.texture, TEXTURE_FILTER_BILINEAR);
}

void game::handleEvents() {
    // Left-click
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePosn = GetMousePosition();

        // If clicked on a spot
        if (mousePosn.x>=GRID_BEGIN_X&&mousePosn.x<=GRID_BEGIN_X+SPOT_COLS*SPOT_SIZE &&
            mousePosn.y>=GRID_BEGIN_Y&&mousePosn.y<=GRID_BEGIN_Y+SPOT_ROWS*SPOT_SIZE) {

            // Get the index of spot clicked on, along with money received for that click
            int i = (mousePosn.x-GRID_BEGIN_X)/SPOT_SIZE;
            int j = (mousePosn.y-GRID_BEGIN_Y)/SPOT_SIZE;
            int receivedAmt = spots[i*SPOT_ROWS+j].mine(mineStrength);
            amt+=receivedAmt;
        
            // On successful breaking of ground/accumulation of treasure
            if (amt>0) {
                // Initialise the fadeout effect required
                fadeout f;
                if (receivedAmt==10) {
                    f.initialise("+$10", YELLOW, {(float)GRID_BEGIN_X+i*SPOT_SIZE, (float)GRID_BEGIN_Y+j*SPOT_SIZE-FONT_SIZE_FADEOUT_SMALL-BUFFER_GRID_AMOUNT}, {0,-1}, 100, FONT_SIZE_FADEOUT_SMALL);
                
                    // Swap texture of mined spot with something else
                    std::random_device rd;                          // seeds from system entropy
                    std::mt19937 gen(rd());                         // Mersenne Twister RNG
                    std::uniform_int_distribution<int> dist(0, SPOT_TEXTURES_COUNT-1);
                
                    spots[i*SPOT_ROWS+j].texture = spotTextures[dist(gen)];
                } else if (receivedAmt==25) {
                    f.initialise("+$25", RED, {(float)GRID_BEGIN_X+i*SPOT_SIZE, (float)GRID_BEGIN_Y+j*SPOT_SIZE-FONT_SIZE_FADEOUT_MEDIUM-BUFFER_GRID_AMOUNT}, {0,-1}, 100, FONT_SIZE_FADEOUT_MEDIUM);
                    resources[0]++;
                } else if (receivedAmt==150) {
                    f.initialise("+$150", ORANGE, {(float)GRID_BEGIN_X+i*SPOT_SIZE, (float)GRID_BEGIN_Y+j*SPOT_SIZE-FONT_SIZE_FADEOUT_MEDIUM-BUFFER_GRID_AMOUNT}, {0,-1}, 100, FONT_SIZE_FADEOUT_MEDIUM);
                    resources[1]++;
                } else if (receivedAmt==1000) {
                    f.initialise("+$1000", GREEN, {(float)GRID_BEGIN_X+i*SPOT_SIZE, (float)GRID_BEGIN_Y+j*SPOT_SIZE-FONT_SIZE_FADEOUT_LARGE-BUFFER_GRID_AMOUNT}, {0,-1}, 100, FONT_SIZE_FADEOUT_LARGE);
                    resources[2]++;
                } else if (receivedAmt==7500) {
                    f.initialise("+$7500", BLUE, {(float)GRID_BEGIN_X+i*SPOT_SIZE, (float)GRID_BEGIN_Y+j*SPOT_SIZE-FONT_SIZE_FADEOUT_LARGE-BUFFER_GRID_AMOUNT}, {0,-1}, 100, FONT_SIZE_FADEOUT_LARGE);
                    resources[3]++;
                } else if (receivedAmt==75000) {
                    f.initialise("+$75000", GOLD, {(float)GRID_BEGIN_X+i*SPOT_SIZE, (float)GRID_BEGIN_Y+j*SPOT_SIZE-FONT_SIZE_FADEOUT_GIGANTIC-BUFFER_GRID_AMOUNT}, {0,-1}, 100, FONT_SIZE_FADEOUT_GIGANTIC);
                    resources[4]++;
                }
                fadeouts.push_back(f);

                // Check the shades of the spots that will be affected by the mining
                getShadow(i*SPOT_ROWS+j);
                getShadow(i*SPOT_ROWS+j+1);
                getShadow(i*SPOT_ROWS+j+SPOT_ROWS);
                getShadow(i*SPOT_ROWS+j+1+SPOT_ROWS);
            }
        }
    }

    Vector2 mousePosn = GetMousePosition();

    // Highlight and unhighlight spots that the cursor hovers over
    if (mousePosn.x>=GRID_BEGIN_X&&mousePosn.x<=GRID_BEGIN_X+SPOT_COLS*SPOT_SIZE &&
        mousePosn.y>=GRID_BEGIN_Y&&mousePosn.y<=GRID_BEGIN_Y+SPOT_ROWS*SPOT_SIZE) {

        // Highlight spot over which mouse is hovering
        int i = mousePosn.x-GRID_BEGIN_X; i/=SPOT_SIZE;
        int j = mousePosn.y-GRID_BEGIN_Y; j/=SPOT_SIZE;
        spots[i*SPOT_ROWS+j].cursorState=1;

        // Unhighlight surrounding spots
        if (i>0) {
            spots[(i-1)*SPOT_ROWS+j].cursorState=0;
            if (j>0) spots[(i-1)*SPOT_ROWS+(j-1)].cursorState=0;
            if (j<SPOT_COLS-1) spots[(i-1)*SPOT_ROWS+(j+1)].cursorState=0;
        }
        if (i<SPOT_ROWS-1) {
            spots[(i+1)*SPOT_ROWS+j].cursorState=0;
            if (j>0) spots[(i+1)*SPOT_ROWS+(j-1)].cursorState=0;
            if (j<SPOT_COLS-1) spots[(i+1)*SPOT_ROWS+(j+1)].cursorState=0;
        }
        if (j>0) spots[i*SPOT_ROWS+(j-1)].cursorState=0;
        if (j<SPOT_COLS-1) spots[i*SPOT_ROWS+(j+1)].cursorState=0;
    }
}

void game::frameProcess() {
    // Calculate the position and intensity of fadeouts, erase them if necessary
    for (auto& f : fadeouts) f.run();
    if (fadeouts.size()>0 && fadeouts[0].currFrame>fadeouts[0].frameCount) fadeouts.erase(fadeouts.begin());

    // Periodically unhighlight all spots
    if (frame%8==0) {
        //std::cout << TextLength("oaure");
        for (auto& spot : spots) spot.cursorState=0;
    }

    frame++;
}

void game::getShadow(int i) {
    if (i>=SPOT_ROWS) spots[i].shade.topShadow=spots[i].depth-spots[i-SPOT_ROWS].depth;
    else spots[i].shade.topShadow=spots[i].depth;

    if (i%SPOT_ROWS>=1) spots[i].shade.leftShadow=spots[i].depth-spots[i-1].depth;
    else spots[i].shade.leftShadow=spots[i].depth;
    
    if (i>=SPOT_ROWS&&i%SPOT_ROWS>=1) spots[i].shade.tlShadow=spots[i].depth-spots[i-SPOT_ROWS-1].depth;
    else spots[i].shade.tlShadow=spots[i].depth;
}

void game::draw() {
    // Background and grid border
    DrawTexture(background, GRID_BEGIN_X-410, GRID_BEGIN_Y-395, WHITE);
    DrawRectangleLinesEx({GRID_BEGIN_X-SPOT_BORDER_THICKNESS, GRID_BEGIN_Y-SPOT_BORDER_THICKNESS, SPOT_COLS*SPOT_SIZE+2*SPOT_BORDER_THICKNESS, SPOT_ROWS*SPOT_SIZE+2*SPOT_BORDER_THICKNESS}, SPOT_BORDER_THICKNESS, BLACK);
    
    // Title card
    std::string title = "Digster v1.0";
    const Vector2 titleBoxBegin = {50,50};
    const float titleBoxWidth = WINDOW_WIDTH-2*titleBoxBegin.x;
    const int titleFontSize = 150;
    const int titleBoxBuffer = 20;
    DrawRectangle(titleBoxBegin.x, titleBoxBegin.y, titleBoxWidth, titleFontSize+2*titleBoxBuffer, {200,9,224, 200});
    DrawRectangleLinesEx({titleBoxBegin.x, titleBoxBegin.y, titleBoxWidth, titleFontSize+2*titleBoxBuffer}, 3, {200,90,224, 255});
    DrawTextEx(titleFont, title.c_str(), {(float)(WINDOW_WIDTH-MeasureTextEx(titleFont, title.c_str(), titleFontSize, 0).x)/2, titleBoxBegin.y+titleBoxBuffer}, titleFontSize, 0, GREEN);
    
    // Resource-count rectangle
    const int rightBoxBuffer = (WINDOW_WIDTH-GRID_BEGIN_X-SPOT_COLS*SPOT_SIZE-RIGHT_BOX_WIDTH)/2;
    const int rightBoxBegin = GRID_BEGIN_X+SPOT_COLS*SPOT_SIZE+rightBoxBuffer;
    DrawRectangle(rightBoxBegin, GRID_BEGIN_Y, RIGHT_BOX_WIDTH, 7*SPOT_SIZE, {200,9,224, 200});
    DrawRectangleLinesEx({rightBoxBegin, GRID_BEGIN_Y, RIGHT_BOX_WIDTH, 7*SPOT_SIZE}, 3, {200,90,224, 255});
    
    // Draw amount possessed in resource-count rectangle
    DrawText(("  " + std::to_string(amt)).c_str(), rightBoxBegin+BUFFER_GRID_AMOUNT, GRID_BEGIN_Y+BUFFER_GRID_AMOUNT, AMOUNT_FONT_SIZE, GREEN);
    DrawText("$", rightBoxBegin+BUFFER_GRID_AMOUNT, GRID_BEGIN_Y+BUFFER_GRID_AMOUNT, AMOUNT_FONT_SIZE, GREEN);
    
    for (int i = 0; i < 5; i++) DrawText(("  " + std::to_string(resources[i])).c_str(), rightBoxBegin+BUFFER_GRID_AMOUNT, GRID_BEGIN_Y+BUFFER_GRID_AMOUNT+(i+1)*(AMOUNT_FONT_SIZE+BUFFER_GRID_AMOUNT), AMOUNT_FONT_SIZE, GREEN);
    treasure treasResrcDraw;
    treasResrcDraw.posn = {rightBoxBegin+BUFFER_GRID_AMOUNT, GRID_BEGIN_Y+BUFFER_GRID_AMOUNT+7};
    for (int i = 0; i < 5; i++) {
        treasResrcDraw.initialise(i+1);
        treasResrcDraw.posn.y+=AMOUNT_FONT_SIZE+BUFFER_GRID_AMOUNT;
        treasResrcDraw.draw();
    }

    // Draw spots
    for (int i = 0; i < SPOT_ROWS; i++) {
        for (int j = 0; j < SPOT_COLS; j++) {
            spots[i*SPOT_ROWS+j].draw(defaultFont);
        }
    }

    // Draw fadeouts
    for (auto f : fadeouts) f.draw();

    // Debug mode
    if (IsKeyDown(KEY_D)) {
        for (auto i : spots) i.debugShow();
        Vector2 posn = GetMousePosition();
        DrawTextEx(defaultFont, (std::to_string(posn.x)+","+std::to_string(posn.y)).c_str(), {20,20}, 25, 0, RED);
        DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 300, 20, 30, RED);
    }

    // Draw the cursor
    Vector2 mousePosn = GetMousePosition();
    if (mousePosn.x>GRID_BEGIN_X&&mousePosn.x<GRID_BEGIN_X+SPOT_COLS*SPOT_SIZE&&mousePosn.y>GRID_BEGIN_Y&&mousePosn.y<GRID_BEGIN_Y+SPOT_ROWS*SPOT_SIZE) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) DrawTexture(cursorTextures[2], mousePosn.x, mousePosn.y-10, PINK);
        else DrawTexture(cursorTextures[1], mousePosn.x+5, mousePosn.y-15, PINK);
    } else DrawTexture(cursorTextures[0], mousePosn.x, mousePosn.y, WHITE);
}

// Unload all textures
void game::close() {
    for (auto& i : spots) i.close();
    for (auto& i : spotTextures) UnloadTexture(i);
    for (auto& i : cursorTextures) UnloadTexture(i);
    UnloadFont(defaultFont);
}