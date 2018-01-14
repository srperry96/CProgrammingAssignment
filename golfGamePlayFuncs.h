#ifndef __GOLFGAMEPLAYFUNCS_H__
#define __GOLFGAMEPLAYFUNCS_H__

#include <math.h>
#include <golfGameGraphicsFuncs.h>
#include <golfGameHighscoreFuncs.h>

/*Run through 9 turns of gameplay, with 3 turns on each level.
This is the main game function which sets up the game and calls
the relevant functions to play a round of golf*/
void playGame(int resX, int resY, int bgColor, int fgColor, int difficulty);

/*Get the launch angle for the current turn. Selection is done
using the mouse position. An arrow is drawn on the screen to show
the current angle. The angle is selected by clicking.*/
float getLaunchAngle(int resX, int resY, int stickmanXPos, int fgColor, int level, ObstacleTree tree, int windSpeed, int difficulty);

/*Get the power to be used in velocity calculations for the current turn.
Power selection is done using the mouse and a graphical power bar on screen*/
void getPower(float *velX, float *velY, int stickmanXPos, int resX, int resY, int fgColor, float launchAngle, int level, ObstacleTree tree, int windSpeed, int difficulty);

/*If there is an obstacle (tree or water) in the current level, test if it has been hit,
and return the type of collision that has been detected. Three types of collision are used:
1 - hitting a vertical obstacle, 2 - hitting a horizontal obstacle from above,
3 - hitting a horizontal obstacle from below, 4 - landing in water*/
int checkObstacleHit(int resX, int resY, int posX, int posY, int level, ObstacleTree tree, int difficulty, int direction, int falling);

/*Calculate the score for the current turn based on the final position of the ball*/
int calculateScore(int landingPos, int resX);

/*Wait for the user to click anywhere on screen*/
void waitForClick(void);

/*Initialise graphics window and the mouse and keyboard event queues*/
void setup(int resX, int resY);

/*Close the mouse and keyboard event queues and graphics window*/
void closeEverything(void);

#endif //__GOLFGAMEPLAYFUNCS_H__
