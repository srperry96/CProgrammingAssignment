#ifndef __GOLFGAMEPLAYFUNCS_H__
#define __GOLFGAMEPLAYFUNCS_H__

#include <math.h>
#include <golfGameGraphicsFuncs.h>
#include <golfGameHighscoreFuncs.h>

void playGame(int resX, int resY, int bgColor, int fgColor);
float getLaunchAngle(int resX, int resY, int stickmanXPos, int fgColor, int level, ObstacleTree tree);
void getPower(float *velX, float *velY, int stickmanXPos, int resX, int resY, int fgColor, float launchAngle, int level, ObstacleTree tree);
int checkObstacleHit(int resX, int resY, int posX, int posY, int level, ObstacleTree tree);
int calculateScore(int landingPos, int resX);

void waitForClick(void);
void setup(int resX, int resY);
void closeEverything(void);

#endif //__GOLFGAMEPLAYFUNCS_H__
