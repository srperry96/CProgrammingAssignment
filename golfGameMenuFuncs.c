#include <golfGameMenuFuncs.h>

/*Initialise buttons for a specified menu screen, draw the menu
and return the player selection when a button is clicked*/
int getMenuSelection(int resX, int resY, int menuID)
{
    int i, spaceBetweenButtons, numMenuItems, mouseXPos, mouseYPos;
    int buttonHeight = resY / 10;
    int buttonWidth = resX / 5;
    MenuButton buttonsArray[4];

    cleardevice();

    if(menuID == 1) //menuID 1 is main menu
    {
        numMenuItems = 4;
        outtextxy(((resX / 2) - 40), 50, "Main Menu");
        strcpy(buttonsArray[0].buttonText, "Play Game");
        strcpy(buttonsArray[1].buttonText, "Highscores");
        strcpy(buttonsArray[2].buttonText, "Settings");
        strcpy(buttonsArray[3].buttonText, "Quit Game");
    }
    else if(menuID == 2)//menuID 2 is settings menu
    {
        numMenuItems = 3;
        outtextxy(((resX / 2) - 40), 50, "Settings");
        strcpy(buttonsArray[0].buttonText, "Colour 1");
        strcpy(buttonsArray[1].buttonText, "Colour 2");
        strcpy(buttonsArray[2].buttonText, "Reset Scores");
    }

    //(resY - 2 * buttonHeight) gives space for all buttons. Divide by num of buttons to get space on screen
    //for each button and the space following it. subtract button height to get space between buttons
    spaceBetweenButtons = ((resY - (2 * buttonHeight)) / numMenuItems) - buttonHeight;

    //set up and draw all buttons
    for(i = 0; i < numMenuItems; i++)
    {
        buttonsArray[i].height = buttonHeight;
        buttonsArray[i].width = buttonWidth;
        //buttons at centre of screen in x axis
        buttonsArray[i].topLeftX = (resX / 2) - (buttonWidth / 2);
        //(2 * buttonHeight) above first button leaves space for the menu title,
        //then button height + space between buttons * i to get Y position for current button
        buttonsArray[i].topLeftY = (2 * buttonHeight) + (i * (buttonHeight)) + (i * spaceBetweenButtons);
        drawButton(buttonsArray[i]);
    }

    //if in settings menu, draw a back button
    MenuButton backButton;
    if(menuID == 2)
    {
        //draw back button

        backButton.width = resX / 12;
        backButton.height = resY / 12;
        backButton.topLeftX = 20;
        backButton.topLeftY = 20;
        strcpy(backButton.buttonText, "Back");

        drawButton(backButton);
    }

    update_display();

    //wait for a button to be clicked
    while(1)
    {
        wait_for_event();

        if(event_mouse_position_changed())
        {
                get_mouse_coordinates();
                mouseXPos = XMOUSE;
                mouseYPos = YMOUSE;
        }
        else if(event_mouse_left_button_down())
        {
            //if mouse is in correct x range to be over a button
            if((mouseXPos >= buttonsArray[0].topLeftX) && (mouseXPos <= (buttonsArray[0].topLeftX + buttonWidth)))
            {
                //loop through the buttons to see if mouse is in y range. if so, return value of that button
                for(i = 0; i < numMenuItems; i++)
                {
                    if((mouseYPos >= buttonsArray[i].topLeftY) && (mouseYPos <= buttonsArray[i].topLeftY + buttonHeight))
                    {
                        return i;
                    }
                }
            }
            else if(menuID == 2) //if we are in the settings menu, check if back button is pressed
            {
                //if mouse is over back button, return -1 so we return to main menu
                if((mouseXPos >= backButton.topLeftX) && (mouseXPos <= (backButton.topLeftX + backButton.width))
                    && (mouseYPos >= backButton.topLeftY) && (mouseYPos <= backButton.topLeftY + backButton.height))
                    return -1;
            }
        }
    }
}

/*Settings menu. Calls the corresponding function depending on the user selection*/
void settingsMenu(int resX, int resY, int *bgColor, int *fgColor)
{
    int menuSelection;
    int backFlag = 0;

    do
    {
        //draw settings menu and wait for the user to choose an option
        menuSelection = getMenuSelection(resX, resY, 2);
        switch(menuSelection)
        {
            case 0: changeColors(0, fgColor, resX, resY);
                    break;
            case 1: changeColors(1, bgColor, resX, resY);
                    break;
            case 2: resetHighScores();
                    cleardevice();
                    outtextxy(resX/2 - 60, resY/2 - 20, "Highscores Reset");
                    update_display();
                    pausefor(800);
                    break;
            case -1: backFlag = 1;
                     break;
            default: break;
        }
    }while(backFlag != 1);//stay in the settings menu until the back button is pressed
}
