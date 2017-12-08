#include <golfGameGraphicsFuncs.h>
#include <golfGameHighscoreFuncs.h>
#include <golfGameMenuFuncs.h>
#include <golfGamePlayFuncs.h>

int main(void)
{
    int resX = 800, resY = 600, exitFlag = 0; //Window resolution variables and flag for if exit has been clicked
    int bgColor = 0; //background colour variable. 0 1 2 3, black, blue, lightgray, brown respectively
    int fgColor = 0; //foreground colour variable. 0 1 2 3, white, red, cyan, yellow respectively

    setup(resX, resY);

    while(exitFlag == 0) //loop until exit game is pressed
    {
        cleardevice();
        //get menu button selection (parameter 1 means main menu)
        int menuSelection = getMenuSelection(resX, resY, 1);

        switch(menuSelection)
        {
            case 0: playGame(resX, resY, bgColor, fgColor);
                    break;
            case 1: showHighScores(resX, resY);
                    break;
            case 2: settingsMenu(resX, resY, &bgColor, &fgColor);
                    break;
            case 3: exitFlag = 1;
                    break;
            default:break;
        }
    }

    closeEverything();

    return 0;
}
