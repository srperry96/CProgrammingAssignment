#include <golfGamePlayFuncs.h>

/*Run through 9 turns of gameplay, with 3 turns on each level.
This is the main game function which sets up the game and calls
the relevant functions to play a round of golf*/
void playGame(int resX, int resY, int bgColor, int fgColor)
{
    int level = 1; //variable to indicate what level to load / what graphics to display
    int stickmanXPos = 70; //initial stickman position - allows space for full swing back of golf club
    int windSpeed = 0;
    int i = 0, landingPos = 0, score = 0;
    float velX = 60.0, velY = 60.0; //launch velocity components
    float launchAngle = 0.0;
    char scoreString[50];

    //setup the tree obstacle for use in level 2
    ObstacleTree tree;
    tree.trunkLeftX = resX/2 - 50;
    tree.trunkHeight = 150;
    tree.trunkWidth = 50;
    tree.leafBottomY = resY - 50 - 110;
    tree.leafLeftX = tree.trunkLeftX - 45;
    tree.leafWidth = 140;
    tree.leafHeight = 80;
    tree.totalHeight = 220;

    //loop for 9 turns, 3 on each level
    for(i = 0; i < 9; i++)
    {
        //calculate level number - we play 3 balls per level, so i/3 + 1 gives level number 1, 2 or 3
        level = (i / 3) + 1;

        //calculate random wind speed (between -5 and 5) for the turn
        //positive windSpeed is left, negative is right
        windSpeed = 5 - (rand() % 10);

        //draw initial screen for this level
        redrawEverything(stickmanXPos, resX, resY, fgColor);
        drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
        drawObstacles(level, resX, resY, tree);
        drawLevelLabels(level, resX, windSpeed);

        //draw instructions if we are on the first turn,
        //or warn about obstacles if we change to a new level,
        //wait for a click before continuing
        if(i == 0)
        {
            outtextxy(resX/2 - 250, resY / 3 - 30, "Move the mouse and click to select shot angle and power.");
            outtextxy(resX/2 - 250, resY / 3, "Be careful, wind speed will have an effect on your shot.");
            outtextxy(resX/2 - 100, resY / 3 + 30, "Click to continue...");
            update_display();
            waitForClick();
        }
        else if(i == 3)
        {
            outtextxy(resX/2 - 200, resY / 3 - 30, "Careful, there is now an obstacle in your way.");
            outtextxy(resX/2 - 160, resY / 3, "You'll have to aim to chip over it.");
            outtextxy(resX/2 - 100, resY / 3 + 30, "Click to continue...");
            update_display();
            waitForClick();
        }
        else if(i == 6)
        {
            outtextxy(resX/2 - 250, resY / 3 - 30, "Watch out! There is now water between you and the target");
            outtextxy(resX/2 - 200, resY / 3, "If the ball lands in the water it will sink.");
            outtextxy(resX/2 - 100, resY / 3 + 30, "Click to continue...");
            update_display();
            waitForClick();
        }
        update_display();

        //get launch angle and power for this turn
        launchAngle = getLaunchAngle(resX, resY, stickmanXPos, fgColor, level, tree, windSpeed);
        getPower(&velX, &velY, stickmanXPos, resX, resY, fgColor, launchAngle, level, tree, windSpeed);

        //incorporate wind in x velocity component (subtract as positive wind goes left, against the positive velX)
        //windSpeed is scaled so it actually affects the ball travel
        velX -= (windSpeed * 3);

        //draw ball movement for this turn and determine its landing position
        landingPos = drawShot(stickmanXPos, resX, resY, velX, velY, bgColor, fgColor, level, tree, windSpeed);

        //flush the event queue so the game waits for user input correctly
        //if this is not done and the player clicks during animation, the game will skip forward too fast
        while(check_if_event()) wait_for_event();

        //calculate score, display it, and wait for a click to continue
        score += calculateScore(landingPos, resX);
        sprintf(scoreString, "Current Score: %d. Click to continue", score);
        outtextxy(resX / 2 - 150, resY/3, scoreString);
        update_display();
        waitForClick();
    }

    showEndScreen(resX, resY, score);

    //check if the player has set a new high score, then display the high scores list
    checkAndSetNewHighScore(score, resX, resY);
    showHighScores(resX, resY);
}

/*Get the launch angle for the current turn. Selection is done
using the mouse position. An arrow is drawn on the screen to show
the current angle. The angle is selected by clicking.*/
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

            //calculate the differences between the centre of the golf ball and the mouse position
            xVal = mouseX - (stickmanXPos + 5); //stickmanXPos + 5 is centre of golf ball
            yVal = (resY - 50 - 5) - mouseY;    //resY-50-5 is centre of golf ball

            //calculate an angle between the golf ball and the mouse position
            angle = atan2(yVal, xVal);

            //limit angle between 5 and 85 degrees (anticlockwise from horizontal) so ball travels properly
            if(angle < M_PI_2 / 18) angle = M_PI_2 / 18;
            else if(angle >= M_PI_2 - (2 * M_PI_2 / 18)) angle = M_PI_2 - (2 * M_PI_2 / 18);

            //redraw window with the angle selection arrow at the correct position
            redrawEverything(stickmanXPos, resX, resY, fgColor);
            drawAngleArrow(stickmanXPos, resY, angle);
            drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
            drawObstacles(level, resX, resY, tree);
            drawLevelLabels(level, resX, windSpeed);
            update_display();
        }
        //mouse button is clicked, so continue
        else if(event_mouse_button_down())
        {
            //redraw everything and break out of the loop to continue
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

/*Get the power to be used in velocity calculations for the current turn.
Power selection is done using the mouse and a graphical power bar on screen*/
void getPower(float *velX, float *velY, int stickmanXPos, int resX, int resY, int fgColor, float launchAngle, int level, ObstacleTree tree, int windSpeed)
{
    int mouseY, i, power = 0;
    float angle = M_PI_2;

    //draw initial power bar based on the y coordinate of the mouse
    //this uses the same loop as in the while loop below, which is commented in detail
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

        //if the mouse moves, test to see if the power bar needs to be changed
        if(event_mouse_position_changed())
        {
            get_mouse_coordinates();
            mouseY = YMOUSE;

            //loop through the 18 regions of the y axis to get a value for power
            //(split into 18 to give an angle of 5 degrees per power level, which
            //is used in the drawing of the golf club swinging back)
            for(i = 0; i < 18; i++)
            {
                if((mouseY >= (i * resY / 18)) && (mouseY < (i + 1)*(resY / 18)))
                {
                    power = i+1;
                    break;
                }
            }
            //calculate the angle the club should be drawn at for this power level
            angle = ((M_PI_2 / 18) * (power));

            //redraw window with club in correct position
            redrawEverything(stickmanXPos, resX, resY, fgColor);
            drawArmsAndClub(stickmanXPos, resY, angle, fgColor);
            drawPowerBars(19 - power, resY);
            drawObstacles(level, resX, resY, tree);
            drawLevelLabels(level, resX, windSpeed);
            update_display();
        }
        //mouse button is clicked, so continue
        else if(event_mouse_button_down())
        {
            //animate the golf club swinging from its current position to its neutral position
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
            //break out of the loop to continue
            break;
        }
    }
    //power regions are calculated in the wrong order as y coordinate increase as you move down
    //so we have power zone 1 at the top, and 18 at the bottom. 19 - power gives the correct power value
    power = 19 - power;

    //scale the power so it affects the velocity significantly enough to make the game playable
    power *= 5;

    //calculate velocity x and y components
    *velX = power * cos(launchAngle);
    *velY = power * sin(launchAngle);
}

/*If there is an obstacle (tree or water) in the current level, test if it has been hit,
and return the type of collision that has been detected. Three types of collision are used:
1 - hitting a vertical obstacle, 2 - hitting a horizontal obstacle from above,
3 - hitting a horizontal obstacle from below, 4 - landing in water*/
int checkObstacleHit(int resX, int resY, int posX, int posY, int level, ObstacleTree tree)
{
     //if the ball hits the right side of the screen, it bounces back
    if(posX > resX - 4)
        return 1;
    //if we are on level 2, there is a tree, so check for collisions with it
    else if(level == 2)
    {
        //this series of conditions tests various possible collisions with each element of the tree
        //hitting the trunk from the left
        if(((posX >= tree.trunkLeftX - 10) && (posX < tree.trunkLeftX) && (posY >= tree.leafBottomY) && (posY < resY - 50))
        //hitting the trunk from the right
        || ((posX > tree.trunkLeftX + tree.trunkWidth) && (posX <= tree.trunkLeftX + tree.trunkWidth + 10) && (posY >= tree.leafBottomY) && (posY < resY - 50))
        //hitting the leaves from the left
        || ((posX >= tree.leafLeftX) && (posX < tree.leafLeftX + 10) && (posY > tree.leafBottomY - tree.leafHeight - 10) && (posY < tree.leafBottomY))
        //hitting the leaves from the right
        || ((posX > tree.leafLeftX + tree.leafWidth - 10) && (posX < tree.leafLeftX + tree.leafWidth) && (posY > tree.leafBottomY - tree.leafHeight) && (posY < tree.leafBottomY))
        //hitting the top (central) leaves from the left
        || ((posX > tree.trunkLeftX - 20) && (posX < tree.trunkLeftX - 10) && (posY > resY - 50 - tree.totalHeight - 10) && (posY < tree.leafBottomY - tree.leafHeight - 15))
        //hitting the top (central) leaves from the right
        || ((posX > tree.trunkLeftX + tree.trunkWidth + 5) && (posX < tree.trunkLeftX + tree.trunkWidth + 15) && (posY > resY - 50 - tree.totalHeight - 10) && (posY < tree.leafBottomY - tree.leafHeight - 15)))
                return 1;
        //hitting the top (central) leaves from the top
        else if(((posX > tree.trunkLeftX - 15) && (posX < tree.trunkLeftX + tree.trunkWidth + 15) && (posY >= resY - 50 - tree.totalHeight - 15) && (posY < resY - 50 - tree.totalHeight))
        //hitting the side leaves from the top
        || ((posX > tree.leafLeftX) && (posX < tree.leafLeftX + tree.leafWidth) && (posY > tree.leafBottomY - tree.leafHeight - 15) && (posY < tree.leafBottomY - tree.leafHeight)))
            return 2;
        //hitting the side leaves from the bottom
        else if((posX > tree.leafLeftX) && (posX < tree.leafLeftX + 40) && (posY < tree.leafBottomY + 10) && (posY >= tree.leafBottomY))
            return 3;
    }
    //if we are on level 3 there is water, so test if we have landed in it
    else if(level == 3)
        //the positions used here are hard coded with the same values as where the water obstacle is drawn,
        //but with a buffer of 5 pixels either side so the ball only sinks if it is entirely over the water
        if((posY >= resY - 55) && (posX >= resX / 2 - 200 + 5) && (posX < resX / 2 + 100 - 5))
            return 4;

    return 0;
}

/*Calculate the score for the current turn based on the final position of the ball*/
int calculateScore(int landingPos, int resX)
{
    //resX - 200 is the left most point of the target. target zones are 30px wide. score zones: 10 20 30 20 10
    int targetZoneBands[6];
    int i, scoreZone = 0;

    //calculate bands for scoring zones
    for(i = 0; i < 6; i++)
        targetZoneBands[i] = (i*30) + resX - 200;

    //test ball position against target zone bands to determine a scoring zone
    for(i = 0; i < 5; i++)
        if( (landingPos > targetZoneBands[i]) && (landingPos <= targetZoneBands[i+1]) )
        {
            scoreZone = i + 1;
            break; //break out of loop if zone has been found - no need to loop through remaining bands
        }

    //determine and return the player score from scoreZone
    switch(scoreZone)
    {
        case 1:
        case 5: return 10;
        case 2:
        case 4: return 20;
        case 3: return 30;
        default: return 0;
    }
}

/*Wait for the user to click anywhere on screen*/
void waitForClick(void)
{
    while(1)
    {
        wait_for_event();

        if(event_mouse_button_down())
            break;
    }
}

/*Initialise graphics window and the mouse and keyboard event queues*/
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

/*Close the mouse and keyboard event queues and graphics window*/
void closeEverything(void)
{
    closemouse();
    closekeyboard();
    closegraph();
}
