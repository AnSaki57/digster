#include "shader.hpp"

void shader::initialise(Vector2 pos, int s) {
    posn = pos;
    size = s;
    topShadow=0;
    leftShadow=0;
    tlShadow=0;
}

void shader::draw() {
    if (topShadow==0&&leftShadow==0&&tlShadow==0) return;

    int t,l,tl;
    t = (topShadow>0) ? topShadow : 0;
    l = (leftShadow>0) ? leftShadow : 0;
    tl = (tlShadow>0) ? tlShadow : 0;
    int contribution = t+l+tl;

    contribution = (contribution<MAXIMUM_SHADOW_CONTRIBUTION/STEPWISE_SHADOW_CONTRIBUTION_INCREMENT) ? contribution : MAXIMUM_SHADOW_CONTRIBUTION/STEPWISE_SHADOW_CONTRIBUTION_INCREMENT;
    DrawRectangle(posn.x, posn.y, size, size, {0,0,0,(unsigned char)(contribution*STEPWISE_SHADOW_CONTRIBUTION_INCREMENT)});
}
