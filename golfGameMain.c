#include <stdio.h>
#include <string.h>
#include <golfGameGraphicsFuncs.h>

typedef struct
{
    int score;
    char name[10];
} NameAndScore;

void moveStickman(int *stickmanXPos, int resX, int resY, int *mouseX, int *mouseY, float launchAngle)
{
    char btnPressed;
    int enterPressed = 0;

    while(1)
    {
        wait_for_event();

        if(event_key_left_arrow())
        {
            if(*stickmanXPos >= 25) //make sure the stickman doesnt go off screen
            {
                *stickmanXPos -= 5;
                redrawEverything(*stickmanXPos, resX, resY);
                drawBow(*stickmanXPos, resY, 60.0, launchAngle);
            }
        }
        else if(event_key_right_arrow())
        {
            if(*stickmanXPos <= (resX/2 - 20) ) // dont let stickman go past (halfway - 20) on screen
            {
                *stickmanXPos += 5;
                redrawEverything(*stickmanXPos, resX, resY);
                drawBow(*stickmanXPos, resY, 60.0, launchAngle);
            }
        }
        else if(event_mouse_position_changed())
        {
            get_mouse_coordinates();
            *mouseX = XMOUSE;
            *mouseY = YMOUSE;
        }
        else if(event_mouse_button_down())
        {
            break;
        }
    }
}

void getLaunchAngle(float *velX, float *velY, int mouseOldX, int mouseOldY, int resX, int resY, int *stickmanXPos, float *angle)
{
    int mouseNewX, mouseNewY;
    int xVal, yVal;
    double power;

    while(1)
    {
        wait_for_event();

        if(event_mouse_position_changed())
        {
            get_mouse_coordinates();
            mouseNewX = XMOUSE;
            mouseNewY = YMOUSE;

            //ensure positive x and y values
            if(mouseOldX >= mouseNewX)
                xVal = mouseOldX - mouseNewX;
            else
                xVal = mouseNewX - mouseOldX;

            if(mouseOldY >= mouseNewY)
                yVal = mouseOldY - mouseNewY;
            else
                yVal = mouseNewY - mouseOldY;


            power = sqrt( (xVal * xVal) + (yVal * yVal) ); //pythagoras to calculate length of launch line


            *angle = atan2(yVal, xVal);


            //redraw window with launch angle line
            redrawEverything(*stickmanXPos, resX, resY);
            drawPowerArrow(mouseOldX, mouseNewX, mouseOldY, mouseNewY, power, *angle);
            drawBow(*stickmanXPos, resY, power, *angle);

            update_display();
        }
        else if(event_mouse_left_button_down()) //mouse button is released, so continue
        {
            cleardevice();
            redrawEverything(*stickmanXPos, resX, resY);
            drawBow(*stickmanXPos, resY, power, *angle);
            break;
        }
    }

    *velX = (power/2) * cos(*angle);
    *velY = (power/2) * sin(*angle);
}

int calculateScore(int landingPos, int resX)
{
    //resX - 200 is start point. target zones are 30px wide. scores 10 20 30 20 10
    int targetZoneBands[6];
    int i, scoreZone = 0;

    for(i = 0; i < 6; i++) //calculate bands for scoring zones
    {
        targetZoneBands[i] = (i*30) + resX - 200;
    }

    for(i = 0; i < 5; i++)
    {
        if( (landingPos > targetZoneBands[i]) && (landingPos <= targetZoneBands[i+1]) )
        {
            scoreZone = i + 1;
            break; //break out of loop if zone has found - saves looping through all remaining bands for no reason
        }
    }

    switch(scoreZone) //determine score from scoreZone and return it
    {
        case 1:
        case 5: return 10;
                break;
        case 2:
        case 4: return 20;
                break;
        case 3: return 30;
                break;
        default: return 0;
                 break;
    }
}

int getMenuSelection(int resX, int resY, int menuID)
{
    int i, spaceBetweenButtons, numMenuItems, mouseXPos, mouseYPos;
    int buttonHeight = resY / 10;
    int buttonWidth = resX / 5;
    MenuButton buttonsArray[4];

    if(menuID == 1) //menuID 1 is main menu
    {
        numMenuItems = 4;
        outtextxy(((resX / 2) - 40), 30, "Main Menu");
        strcpy(buttonsArray[0].buttonText, "Play Game");
        strcpy(buttonsArray[1].buttonText, "Highscores");
        strcpy(buttonsArray[2].buttonText, "Settings");
        strcpy(buttonsArray[3].buttonText, "Quit Game");
        //(resY - 2 * buttonHeight) gives space for all buttons. Divide by num of buttons to get space on screen
        //for each button and the space following it. subtract button height to get space between buttons
        spaceBetweenButtons = ((resY - (2 * buttonHeight)) / numMenuItems) - buttonHeight;
    }
    else if(menuID == 2)//menuID 2 is settings menu
    {
        //BG colour
        //pen colour
        //clear highscores?
    }

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
        }
    }
}

void showHighScores()
{
    NameAndScore namesAndScoresArray[10];

    int i, j;

    //read in high scores to array
    FILE *hiscoresFile = fopen("highscores.txt", "r");
    char nextLetter;

    for(i = 0; i < 10; i++)
    {
        fscanf(hiscoresFile, "%d", &namesAndScoresArray[i].score);
        j = 0;
        while(1)
        {   //read characters until a newline char is reached
            fscanf(hiscoresFile, "%c", &nextLetter);
            if(nextLetter == '\n')
            { //set terminating character of string at current element
                namesAndScoresArray[i].name[j] = '\0';
                break;
            }
            else
            {   //add new char to name string
                namesAndScoresArray[i].name[j] = nextLetter;
                j++;
            }
        }
        //============change this to graphical printout
        printf("\n%d: %s : %d", (i+1), namesAndScoresArray[i].name, namesAndScoresArray[i].score);
    }
    fclose(hiscoresFile);

    /* WRITE TO FILE CODE
    --IF A NEW HIGH SCORE IS SET it will be in namesAndScoresArray[10]
    --need to calculate position in list, then write elements 0 - position,
    --then write new score
    --then write elements position + 1 to 9

    FILE *hiscoresFile = fopen("highscores.txt", "w");
    int i, j;
    for(i = 0; i < 10; i++)
    {
        fprintf(hiscoresFile, "%d", namesAndScoresArray[i].score);
        fprintf(hiscoresFile, namesAndScoresArray[i].name);
        fprintf(hiscoresFile, "\n");
    }
    fclose(hiscoresFile);
    */
}

int main(void)
{
    int resX = 640, resY = 480; //Window resolution variables
    int stickmanXPos = 50; //initial stickman position
    int mouseX = 0, mouseY = 0;
    float velX = 60;
    float velY = 60; //launch velocity components default to 45 degree angle
    float launchAngle = 0.0;

    initwindow(resX, resY); //open graphics window
    initfont();//initialise text
    //getColors(); //get background and pen colors

    initmouse();
    initkeyboard();
    create_event_queue();
    reg_display_events();
    reg_mouse_events();
    reg_keyboard_events();
    //hide_mouse_cursor(); -- maybe create own cursor later on


    //get menu button selection
    int menuSelection = getMenuSelection(resX, resY,1); //1 is main menu
    printf("\n\nselection is %d", menuSelection);

//=========================================
//    if(menuSelection == 1) showHighScores();
//    return 0;
//=====================make menu selection do stuff now==================

    //draw initial screen
    drawStickman(stickmanXPos, resY);
    drawBow(stickmanXPos, resY, 60, launchAngle);
    drawGround(resX, resY);
    drawTarget(resX,resY);
    update_display();

    int i, landingPos, score = 0;

    for(i = 0; i < 3; i++)
    {
        printf("\nMove stickman using left and right keys, then press enter\n");
        moveStickman(&stickmanXPos, resX, resY, &mouseX, &mouseY, launchAngle);
        getLaunchAngle(&velX, &velY, mouseX, mouseY, resX, resY, &stickmanXPos, &launchAngle);
        landingPos = drawArc(stickmanXPos, resX, resY, velX, velY);
        score += calculateScore(landingPos, resX);
        printf("\nCurrent Score: %d\n", score);
    }


    //wait for a click before closing
    while(1)
    {
        wait_for_event();

        if(event_mouse_button_down())
            break;
    }

    //close everything
    closemouse();
    closekeyboard();
    closegraph();

    return 0;
}
