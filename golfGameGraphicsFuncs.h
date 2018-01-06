#ifndef __GOLFGAMEGRAPHICSFUNCS_H__
#define __GOLFGAMEGRAPHICSFUNCS_H__

#include <graphics_lib.h>
#include <math.h>

#define GRAVITY 9.81

/*Structure for menu button. Holds a top left coordinate and
width and height so the button can be drawn, along with a
string element for the button text*/
typedef struct
{
    int width, height;
    int topLeftX, topLeftY;
    char buttonText[20];
} MenuButton;

/*Structure for tree obstacle object. Properties used for drawing
the tree and in collision detection*/
typedef struct
{
    int trunkLeftX, trunkWidth, trunkHeight;
    int leafLeftX, leafBottomY, leafWidth, leafHeight;
    int totalHeight; //variables for ball bounce off calculation
} ObstacleTree;

/*Draw the stickman*/
void drawStickman(int x_position, int y_position, int fgColor);

/*Draw ground rectangle filling the bottom 50 pixels of the screen*/
void drawGround(int resX, int resY);

/*Draw a golf ball (white circle) at the stickmans feet*/
void drawBallAtFeet(int stickmanXPos, int resY);

/*Draw the target on the right of the screen. This consists
of lines and numbers to label the scores for each zone*/
void drawTarget(int resX, int resY, int difficulty);

/*Draw the arms of the stickman and the golfclub at a specified angle*/
void drawArmsAndClub(int startX, int resY, float angle, int fgColor);

/*Clear the device and redraw the ground, stickman, target and ball at the stickmans feet*/
void redrawEverything(int currentXPos, int resX, int resY, int fgColor, int difficulty);

/*Draw four squares of different colours on screen, and change the background
or foreground colour based on the user selection of one of these squares.
colorToChange indicates whether foreground (0) or background (1) colour is being chosen*/
void changeColors(int colorToChange, int *colorValue, int resX, int resY);

/*Calculate the path of the golf ball and animate its motion.
Collisions are checked and a final position is returned when
the ball stops moving or is lost*/
int drawShot(int stickmanXPos, int resX, int resY, float velX, float velY, int bgColor, int fgColor, int level, ObstacleTree tree, int windSpeed, int difficulty);

/*Draw a button on screen in a position specified by the buttons properties*/
void drawButton(MenuButton btn);

/*Draws power bars on the screen, with the number of bars determined
by the power variable. The size and position of the bars is calculated
based on the y resolution of the graphics window*/
void drawPowerBars(int power, int resY);

/*Draw a small arrow at a specified angle. The base of the arrow
is the centre of the golf ball. Trigonometry is used to calculate
x and y coordinates for each element of the arrow*/
void drawAngleArrow(int stickmanXPos, int resY, float angle);

/*Write out GAME OVER and the players final score, then wait for a click*/
void showEndScreen(int resX, int resY, int score);

/*Draw the obstacles required for each level*/
void drawObstacles(int level, int resX, int resY, ObstacleTree tree);

/*Animate the golf ball sinking below where it landed in the water*/
void ballSinkAnimation(int posX, int resX, int resY);

/*Write out the level number and wind speed to the screen*/
void drawLevelLabels(int level, int resX, int windSpeed);
#endif //__GOLFGAMEGRAPHICSFUNCS_H__
