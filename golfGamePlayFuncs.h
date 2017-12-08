#ifndef __GOLFGAMEPLAYFUNCS_H__
#define __GOLFGAMEPLAYFUNCS_H__

#include <math.h>
#include <golfGameGraphicsFuncs.h>
#include <golfGameHighscoreFuncs.h>

float getLaunchAngle(int resX, int resY, int stickmanXPos, int fgColor);
void getPower(float *velX, float *velY, int stickmanXPos, int resX, int resY, int fgColor, float launchAngle);
int calculateScore(int landingPos, int resX);
void waitForClick(void);
void playGame(int resX, int resY, int bgColor, int fgColor);
void setup(int resX, int resY);
void closeEverything(void);

#endif //__GOLFGAMEPLAYFUNCS_H__
