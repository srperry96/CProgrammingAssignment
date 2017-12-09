#include <golfGamePlayFuncs.h>

void playGame(int resX, int resY, int bgColor, int fgColor)
{
    int level = 1; //variable to indicate what level to load / what graphics to display
    int stickmanXPos = 70; //initial stickman position
    int windSpeed = 0;
    float velX = 60, velY = 60; //launch velocity components default to 45 degree angle
    float launchAngle = 0.0;

    //setup tree obstacle
    ObstacleTree tree;
    tree.trunkLeftX = resX/2 - 50;
    tree.trunkHeight = 150;
    tree.trunkWidth = 50;
    tree.leafBottomY = resY - 50 - 110;
    tree.leafLeftX = tree.trunkLeftX - 45;
    tree.leafWidth = 140;
    tree.leafHeight = 80;
    tree.totalHeight = 220;

    int i, landingPos, score = 0;
    char scoreString[50];

    for(i = 0; i < 9; i++)
    {
        //calculate level number - we play 3 balls per level, so i/3
        level = (i / 3) + 1;

        //calculate random wind speed (between -5 and 5) for this turn
        //positive windSpeed is left, negative right
        windSpeed = 5 - (rand() % 10);

        //draw initial screen for this level
        redrawEverything(stickmanXPos, resX, resY, fgColor);
        drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
        drawObstacles(level, resX, resY, tree);
        drawLevelLabels(level, resX, windSpeed);

        //draw instructions on first turn
        if(i == 0)
        {
            outtextxy(resX/2 - 250, resY / 3, "Move the mouse and click to select shot angle and power");
            outtextxy(resX/2 - 100, resY / 3 + 30, "Click to continue");
            update_display();
            waitForClick();
        }
        update_display();

        //get launch angle and power for this turn
        launchAngle = getLaunchAngle(resX, resY, stickmanXPos, fgColor, level, tree, windSpeed);
        getPower(&velX, &velY, stickmanXPos, resX, resY, fgColor, launchAngle, level, tree, windSpeed);

        //incorporate wind in x velocity component (subtract as positive wind goes left, against positive velX)
        //windSpeed is scaled so it actually affects the ball travel
        velX -= (windSpeed * 3);

        //draw ball movement for this turn
        landingPos = drawShot(stickmanXPos, resX, resY, velX, velY, bgColor, fgColor, level, tree, windSpeed);

        //flush event queue so the game waits for user input correctly
        while(check_if_event()) wait_for_event();

        //calculate score, display and wait for a click to continue
        score += calculateScore(landingPos, resX);
        sprintf(scoreString, "Current Score: %d. Click to continue", score);
        outtextxy(resX / 2 - 150, resY/3, scoreString);
        update_display();
        waitForClick();
    }

    showEndScreen(resX, resY, score);

    checkAndSetNewHighScore(score, resX, resY);
    showHighScores(resX, resY);
}

float getLaunchAngle(int resX, int resY, int stickmanXPos, int fgColor, int level, ObstacleTree tree, int windSpeed)
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
            else if(angle >= M_PI_2 - (2 * M_PI_2 / 18)) angle = M_PI_2 - (2 * M_PI_2 / 18);

            //redraw window with launch angle arrow
            redrawEverything(stickmanXPos, resX, resY, fgColor);
            drawAngleArrow(stickmanXPos, resY, angle);
            drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
            drawObstacles(level, resX, resY, tree);
            drawLevelLabels(level, resX, windSpeed);
            update_display();
        }
        else if(event_mouse_button_down()) //mouse button is clicked, so continue
        {
            cleardevice();
            redrawEverything(stickmanXPos, resX, resY, fgColor);
            drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
            drawObstacles(level, resX, resY, tree);
            drawLevelLabels(level, resX, windSpeed);
            update_display();
            break;
        }
    }

    return angle;
}

void getPower(float *velX, float *velY, int stickmanXPos, int resX, int resY, int fgColor, float launchAngle, int level, ObstacleTree tree, int windSpeed)
{
    int mouseY, i, power = 0;
    float angle = M_PI_2;

    //draw initial power bar - this uses the same for loop as below
    get_mouse_coordinates();
    mouseY = YMOUSE;
    for(i = 0; i < 18; i++)
    {
        if((mouseY >= (i * resY / 18)) && (mouseY < (i + 1)*(resY / 18)))
        {
            power = i+1;
            break;
        }
    }
    angle = ((M_PI_2 / 18) * (power));
    redrawEverything(stickmanXPos, resX, resY, fgColor);
    drawArmsAndClub(stickmanXPos, resY, angle, fgColor);
    drawPowerBars(19 - power, resY);
    drawObstacles(level, resX, resY, tree);
    drawLevelLabels(level, resX, windSpeed);
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
            drawObstacles(level, resX, resY, tree);
            drawLevelLabels(level, resX, windSpeed);
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
                drawObstacles(level, resX, resY, tree);
                drawLevelLabels(level, resX, windSpeed);
                update_display();
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

int checkObstacleHit(int resX, int resY, int posX, int posY, int level, ObstacleTree tree)
{
    if(posX > resX - 4) //ball bounces off right side of screen
        return 1;
    else if(level == 2) // level 2 - tree
    {
        //hit trunk from left
        if(((posX >= tree.trunkLeftX - 10) && (posX < tree.trunkLeftX) && (posY >= tree.leafBottomY) && (posY < resY - 50))
        //hit trunk from right
        || ((posX > tree.trunkLeftX + tree.trunkWidth) && (posX <= tree.trunkLeftX + tree.trunkWidth + 10) && (posY >= tree.leafBottomY) && (posY < resY - 50))
        //hit leaf from left
        || ((posX >= tree.leafLeftX) && (posX < tree.leafLeftX + 10) && (posY > tree.leafBottomY - tree.leafHeight - 10) && (posY < tree.leafBottomY))
        //hit leaf from right
        || ((posX > tree.leafLeftX + tree.leafWidth - 10) && (posX < tree.leafLeftX + tree.leafWidth) && (posY > tree.leafBottomY - tree.leafHeight) && (posY < tree.leafBottomY))
        //hit middle leaf from left
        || ((posX > tree.trunkLeftX - 20) && (posX < tree.trunkLeftX - 10) && (posY > resY - 50 - tree.totalHeight - 10) && (posY < tree.leafBottomY - tree.leafHeight - 15))
        //hit middle leaf from right
        || ((posX > tree.trunkLeftX + tree.trunkWidth + 5) && (posX < tree.trunkLeftX + tree.trunkWidth + 15) && (posY > resY - 50 - tree.totalHeight - 10) && (posY < tree.leafBottomY - tree.leafHeight - 15)))
                return 1;
        //hit middle leaf from top
        else if(((posX > tree.trunkLeftX - 15) && (posX < tree.trunkLeftX + tree.trunkWidth + 15) && (posY >= resY - 50 - tree.totalHeight - 15) && (posY < resY - 50 - tree.totalHeight))
        //hit side leaves from top
        || ((posX > tree.leafLeftX) && (posX < tree.leafLeftX + tree.leafWidth) && (posY > tree.leafBottomY - tree.leafHeight - 15) && (posY < tree.leafBottomY - tree.leafHeight)))
            return 2;
        //hit side leaves from bottom
        else if((posX > tree.leafLeftX) && (posX < tree.leafLeftX + 40) && (posY < tree.leafBottomY + 10) && (posY >= tree.leafBottomY))
            return 3;

    }
    else if(level == 3) //level 3 - water
        if((posY >= resY - 55) && (posX >= resX / 2 - 200 + 5) && (posX < resX / 2 + 100 - 5))
            return 4;

    return 0;
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
