#include <stdio.h>
#include <string.h>
#include <golfGameGraphicsFuncs.h>
#include <golfGameHighscoreFuncs.h>

void drawAngleArrow(int stickmanXPos, int resY, float angle)
{
    int lineEndX, lineEndY;

    //draw shaft of arrow
    moveto(stickmanXPos + 5, resY - 50 - 5);
    lineEndX = (stickmanXPos + 5) + (int)(40 * cos(angle));
    lineEndY = (resY - 50 - 5) - (int)(40 * sin(angle));
    lineto(lineEndX, lineEndY, 2);

    //draw head of arrow
    line(lineEndX, lineEndY, (lineEndX - (int)(10 * sin(angle + M_PI_4))), (lineEndY - (int)(10 * cos(angle + M_PI_4))), 2);
    line(lineEndX, lineEndY, (lineEndX + (int)(10 * sin(angle - M_PI_4))), (lineEndY + (int)(10 * cos(angle - M_PI_4))), 2);
}

float getLaunchAngle(int resX, int resY, int stickmanXPos, int fgColor)
{
    int mouseX, mouseY;
    int xVal, yVal;
    double angle = M_PI_4;

    while(1)
    {
        wait_for_event();

        if(event_mouse_position_changed())
        {
            get_mouse_coordinates();
            mouseX = XMOUSE;
            mouseY = YMOUSE;

            xVal = mouseX - (stickmanXPos + 5); //stickmanXPos + 5 is centre of golf ball
            yVal = resY - 50 - 5 - mouseY; //resY-50-5 is centre of golf ball

            angle = atan2(yVal, xVal);

            //limit angle to 90 degrees (anticlockwise from horizontal)
            if(angle < 0) angle = 0;
            else if(angle > M_PI_2) angle = M_PI_2;

            //redraw window with launch angle arrow
            redrawEverything(stickmanXPos, resX, resY, fgColor);
            drawAngleArrow(stickmanXPos, resY, angle);
            drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
            update_display();
        }
        else if(event_mouse_button_down()) //mouse button is clicked, so continue
        {
            cleardevice();
            redrawEverything(stickmanXPos, resX, resY, fgColor);
            drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
            break;
        }
    }

    return angle;
}

void drawPowerBars(int power, int resY)
{
    int height = (resY - 150)/ 18;
    int topY, bottomY, i;

    for(i = 0; i < power; i++)
    {
        bottomY = resY - 140 - ((i-1) * height);
        topY = resY - 140 - (i * height);
        //power bar fill
        filled_rectangle(120, topY, 170, bottomY, RED);
        //power bar border
        rectangle(120, topY, 170, bottomY, 2);
    }
}

void getPower(float *velX, float *velY, int stickmanXPos, int resX, int resY, int fgColor, float launchAngle)
{
    int mouseX, mouseY, i, power = 0;
    float angle = M_PI_2;

    //draw initial power bar
    drawPowerBars(1, resY);
    update_display();

    while(1)
    {
        wait_for_event();

        if(event_mouse_position_changed())
        {
            get_mouse_coordinates();
            mouseX = XMOUSE;
            mouseY = YMOUSE;

            //loop through 18 regions of y axis to get a value for power
            //(split into 18 to give 5 degrees per power level)
            for(i = 0; i < 18; i++)
            {
                if((mouseY >= (i * resY / 18)) && (mouseY < (i + 1)*(resY / 18)))
                {
                    power = i+1;
                    break;
                }
            }


            angle = ((M_PI_2 / 18) * (power));

            //redraw window with club in correct position
            redrawEverything(stickmanXPos, resX, resY, fgColor);
            drawArmsAndClub(stickmanXPos, resY, angle, fgColor);
            drawPowerBars(19 - power, resY);
            update_display();
        }
        else if(event_mouse_button_down()) //mouse button is clicked, so continue
        {
            //animate the golf club swinging from current position to its lowest point
            for(i = power; i < 19; i++)
            {
                angle = ((M_PI_2 / 18) * i);
                cleardevice();
                redrawEverything(stickmanXPos, resX, resY, fgColor);
                drawArmsAndClub(stickmanXPos, resY, angle, fgColor);
                pausefor(5);
            }
            break;
        }
    }
    //power regions calculated in wrong order (1 at top, 9 at bottom), so 19-power gives correct power value
    power = 19 - power;

    //scale power
    power *= 5;
    //calculate velocity x and y components
    *velX = power * cos(launchAngle);
    *velY = power * sin(launchAngle);
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
            break; //break out of loop if zone has been found - saves looping through all remaining bands for no reason
        }
    }

    switch(scoreZone) //determine score from scoreZone and return it
    {
        case 1:
        case 5: return 10;
        case 2:
        case 4: return 20;
        case 3: return 30;
        default: return 0;
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
    }
    else if(menuID == 2)//menuID 2 is settings menu
    {
        numMenuItems = 3;
        outtextxy(((resX / 2) - 40), 30, "Settings");
        strcpy(buttonsArray[0].buttonText, "Colour 1");
        strcpy(buttonsArray[1].buttonText, "Colour 2");
        strcpy(buttonsArray[2].buttonText, "Clear Hiscores");
    }

    //(resY - 2 * buttonHeight) gives space for all buttons. Divide by num of buttons to get space on screen
    //for each button and the space following it. subtract button height to get space between buttons
    spaceBetweenButtons = ((resY - (2 * buttonHeight)) / numMenuItems) - buttonHeight;

    //set up and draw all buttons
    cleardevice();
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

void settingsMenu(int resX, int resY, int *bgColor, int *fgColor)
{
    int menuSelection = getMenuSelection(resX, resY, 2);

    switch(menuSelection)
    {
        case 0: changeColors(0, fgColor, resX, resY);
                break;
        case 1: changeColors(1, bgColor, resX, resY);
                break;
        case 2: resetHighScores();
                cleardevice();
                outtextxy(resX/2 - 50, resY/2 - 20, "Highscores Reset");
                update_display();
                pausefor(800);
                break;
        default: break;
    }
}

void waitForClick(void)
{
    while(1)
    {
        wait_for_event();

        if(event_mouse_button_down())
            break;
    }
}

void showEndScreen(int resX, int resY, int score)
{
    char scoreString[5];

    cleardevice();
    sprintf(scoreString, "%d", score);
    outtextxy(resX / 2 - 80, resY / 2 - 100,"GAME OVER");
    outtextxy(resX / 2 - 80, resY / 2 - 60,"Score: ");
    outtextxy(resX / 2, resY / 2 - 60, scoreString);
    outtextxy(resX / 2 - 120, resY / 2 - 20, "Click to continue");
    update_display();
    waitForClick();
}

void playGame(int resX, int resY, int bgColor, int fgColor)
{
    int stickmanXPos = 70; //initial stickman position
    float velX = 60, velY = 60; //launch velocity components default to 45 degree angle
    float launchAngle = 0.0;

    cleardevice();
    //draw initial screen
    drawStickman(stickmanXPos, resY, fgColor);
    drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
    drawGround(resX, resY);
    drawTarget(resX,resY);
    update_display();

    int i, landingPos, score = 0;
    char scoreString[50];

    for(i = 0; i < 3; i++)
    {
        launchAngle = getLaunchAngle(resX, resY, stickmanXPos, fgColor);
        getPower(&velX, &velY, stickmanXPos, resX, resY, fgColor, launchAngle);
        landingPos = drawArc(stickmanXPos, resX, resY, velX, velY, bgColor, fgColor);
        score += calculateScore(landingPos, resX);
        sprintf(scoreString, "Current Score: %d. Click to continue", score);
        outtextxy(resX / 2 - 150, resY/3, scoreString);
        update_display();
        waitForClick();//ensure previous clicks dont carry through and start next turn accidentally
    }

    showEndScreen(resX, resY, score);

    checkAndSetNewHighScore(score, resX, resY);
    showHighScores(resX, resY);
}

void setup(int resX, int resY)
{
    //graphics setup
    initwindow(resX, resY);
    initfont();
    setcolor(WHITE);
    setbkcolor(BLACK);

    //mouse and keyboard setup
    initmouse();
    initkeyboard();
    create_event_queue();
    reg_display_events();
    reg_mouse_events();
    reg_keyboard_events();
}

void closeEverything()
{
    //close everything
    closemouse();
    closekeyboard();
    closegraph();
}

int main(void)
{
    int resX = 800, resY = 600, exitFlag = 0; //Window resolution variables
    int bgColor = 0; //background colour variable. 0 1 2 3, black, blue, lightgray, brown respectively
    int fgColor = 0; //foreground colour variable. 0 1 2 3, white, red, cyan, yellow respectively

    setup(resX, resY);

    while(exitFlag == 0) //loop until exit game is pressed
    {
        cleardevice();
        //get menu button selection
        int menuSelection = getMenuSelection(resX, resY, 1); //1 is main menu

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
            default:
                break;
        }
    }

    closeEverything();

    return 0;
}
