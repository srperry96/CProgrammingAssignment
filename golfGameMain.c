#include <golfGameGraphicsFuncs.h>
#include <golfGameHighscoreFuncs.h>
#include <golfGameMenuFuncs.h>
#include <golfGamePlayFuncs.h>

/*Main function for the game. Displays the main menu and loops
until exit is pressed. It calls the relevant function depending
on the menu button pressed*/
int main(void)
{
    int resX = 800, resY = 600, exitFlag = 0; //Window resolution variables and flag for if exit has been clicked
    int fgColor = 0; //foreground colour variable. 0 1 2 3, white, red, cyan, yellow respectively
    int bgColor = 0; //background colour variable. 0 1 2 3, black, blue, lightgray, magenta respectively
    int difficulty = 0; // difficulty determines how gameplay works. 0 is easy, 1 is medium, 2 is hard
                        //Easy - hole with no wind; Medium - hole with wind; Hard - target with wind

    setup(resX, resY);

    //loop until exit game is pressed
    while(exitFlag == 0)
    {
        cleardevice();
        //get menu button selection (parameter 1 means show main menu)
        int menuSelection = getMenuSelection(resX, resY, 1);

        switch(menuSelection)
        {
            case 0: difficulty = getMenuSelection(resX, resY, 3);
                    playGame(resX, resY, bgColor, fgColor, difficulty);
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
