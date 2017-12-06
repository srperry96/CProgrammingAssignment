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

void drawStickman(int x_position, int y_position);
void drawGround(int resX, int resY);
void drawTarget(int resX, int resY);
void drawArmsAndClub(int startX, int resY, double power, float angle);
void drawBallAtFeet(int stickmanXPos, int resY);
void redrawEverything(int currentXPos, int resX, int resY);
void getColors(void);
int drawArc(int stickmanXPos, int resX, int resY, float velX, float velY);
void drawPowerArrow(int mouseOldX, int mouseNewX, int mouseOldY, int mouseNewY, double power, float angle);
void drawButton(MenuButton btn);


#endif //__GOLFGAMEGRAPHICSFUNCS_H__
