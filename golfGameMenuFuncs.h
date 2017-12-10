#ifndef __GOLFGAMEMENUFUNCS_H__
#define __GOLFGAMEMENUFUNCS_H__

#include <golfGameGraphicsFuncs.h>
#include <golfGameHighscoreFuncs.h>

/*Initialise buttons for a specified menu screen, draw the menu
and return the player selection when a button is clicked*/
int getMenuSelection(int resX, int resY, int menuID);

/*Settings menu. Calls the corresponding function depending on the user selection*/
void settingsMenu(int resX, int resY, int *bgColor, int *fgColor);

#endif //__GOLFGAMEMENUFUNCS_H__
