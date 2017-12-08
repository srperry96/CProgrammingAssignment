#ifndef __GOLFGAMEGRAPHICSFUNCS_H__
#define __GOLFGAMEGRAPHICSFUNCS_H__

#include <graphics_lib.h>
#include <math.h>

typedef struct
{
    int width, height;
    int topLeftX, topLeftY;
    char buttonText[20];
} MenuButton;

void drawStickman(int x_position, int y_position, int fgColor);
void drawGround(int resX, int resY);
void drawTarget(int resX, int resY);
void drawArmsAndClub(int startX, int resY, float angle, int fgColor);
void drawBallAtFeet(int stickmanXPos, int resY);
void redrawEverything(int currentXPos, int resX, int resY, int fgColor);
void changeColors(int colorToChange, int *colorValue, int resX, int resY);
int drawArc(int stickmanXPos, int resX, int resY, float velX, float velY, int bgColor, int fgColor);
void drawPowerArrow(int mouseOldX, int mouseNewX, int mouseOldY, int mouseNewY, double power, float angle);
void drawButton(MenuButton btn);
void drawPowerBars(int power, int resY);
void drawAngleArrow(int stickmanXPos, int resY, float angle);
void showEndScreen(int resX, int resY, int score);

#endif //__GOLFGAMEGRAPHICSFUNCS_H__
