#include "raylib.h"

//#define GREY_SHADOW {10, 10, 10, 100}
#define MAXIMUM_SHADOW_CONTRIBUTION 150
#define STEPWISE_SHADOW_CONTRIBUTION_INCREMENT 20

class shader {
public:
    Vector2 posn;
    int topShadow;
    int leftShadow;
    int tlShadow;
    int size;

    void initialise(Vector2 pos, int s);
    void draw();
};