#include <golfGamePlayFuncs.h>

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

            //limit angle between 5 and 85 degrees (anticlockwise from horizontal) so ball travels properly
            if(angle < M_PI_2 / 18) angle = M_PI_2 / 18;
            else if(angle >= M_PI_2) angle = M_PI_2 - (M_PI_2 / 18);

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

void getPower(float *velX, float *velY, int stickmanXPos, int resX, int resY, int fgColor, float launchAngle)
{
    int mouseY, i, power = 0;
    float angle = M_PI_2;

    //draw initial power bar - 10 chosen as that is near middle power
    drawPowerBars(10, resY);
    update_display();

    while(1)
    {
        wait_for_event();

        if(event_mouse_position_changed())
        {
            get_mouse_coordinates();
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

void waitForClick(void)
{
    while(1)
    {
        wait_for_event();

        if(event_mouse_button_down())
            break;
    }
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

void closeEverything(void)
{
    //close everything
    closemouse();
    closekeyboard();
    closegraph();
}


