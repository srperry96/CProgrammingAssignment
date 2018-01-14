#include <golfGameGraphicsFuncs.h>
#include <golfGamePlayFuncs.h>

/*Draw the stickman*/
void drawStickman(int xPos, int resY, int fgColor)
{
    //resY -70 for height of stickman, -50 for ground height = -120.
    //this gives us the y position to draw the stickman at
    int yPos = resY - 120;

    //draw head in correct foreground colour
    switch(fgColor)
    {
        case 0: filled_circle(xPos, yPos, 10, WHITE);
                break;
        case 1: filled_circle(xPos, yPos, 10, RED);
                break;
        case 2: filled_circle(xPos, yPos, 10, CYAN);
                break;
        case 3: filled_circle(xPos, yPos, 10, YELLOW);
                break;
        default: break;
    }

    //draw the body, left leg and right leg respectively
    line(xPos, yPos + 10, xPos, yPos + 40, 2);
    line(xPos, yPos + 40, xPos - 10, yPos + 70, 2);
    line(xPos, yPos + 40, xPos + 10, yPos + 70, 2);
}

/*Draw ground rectangle filling the bottom 50 pixels of the screen*/
void drawGround(int resX, int resY)
{
    filled_rectangle(0, resY-50, resX, resY, GREEN);
}

/*Draw a golf ball (white circle) at the stickmans feet*/
void drawBallAtFeet(int stickmanXPos, int resY)
{
    filled_circle(stickmanXPos + 5, resY - 50 - 5, 4, WHITE);
}

/*Draw the target on the right of the screen. This consists
of lines and numbers to label the scores for each zone*/
void drawTarget(int resX, int resY, int difficulty)
{
    if(difficulty != 2) //draw hole for Easy and Medium difficulties
    {
        //draw hole - black rectangle
        filled_rectangle(resX - 140, resY - 50, resX - 110, resY - 30, BLACK);
        //draw flag - white rectangle for pole, red and black triangles for flag and its outline
        filled_rectangle(resX - 127, resY - 150, resX - 123, resY - 30, WHITE);
        filled_triangle(resX - 122, resY - 151, resX - 122, resY - 119, resX - 98, resY - 135, BLACK);
        filled_triangle(resX - 123, resY - 150, resX - 123, resY - 120, resX - 100, resY - 135, RED);
    }
    else //draw target for Hard difficulty
    {
        //initialise scoring zones array
        int scores[5] = {10, 20, 30, 20, 10};
        char strScores[3];
        int i, pos;

        //draw lines and labels for the target. each zone is 30 pixels wide
        for(i = 0; i < 5; i++)
        {
            //resX - 200 is leftmost point of target. draw line every 30 pixels right from there
            pos = (i * 30) + (resX - 200);
            moveto(pos, resY - 50);
            lineto(pos, resY - 40, 1);
            sprintf(strScores, "%d", scores[i]);
            outtextxy(pos + 6, resY - 45, strScores);
        }

        //draw last line of target (right most line)
        pos = (5 * 30) + (resX - 200);
        moveto(pos, resY - 50);
        lineto(pos, resY - 40, 1);
    }
}

/*Draw the arms of the stickman and the golfclub at a specified angle*/
void drawArmsAndClub(int startX, int resY, float angle, int fgColor)
{
    //angle needs Pi/4 subtracted in order to be used for the drawing calculations
    angle -= M_PI_4;

    //calculate lengths of upper arm in x and y for the specified angle
    //these also correspond to the lengths of the forearm, but in reverse (x is y, and y is x)
    int lengthX = (int)(20 * cos(angle));
    int lengthY = (int)(20 * sin(angle));

    //calculate y position of shoulder joint (120 is ground height + stickman height. +15 to get to shoulder point)
    int startY = resY - 120 + 15;

    //draw left arm
    moveto(startX, startY);
    //upper arm
    lineto(startX - lengthX, startY + lengthY, 2);
    //forearm - x and y additions are reversed as mentioned previously
    lineto(startX - lengthX + lengthY, startY + lengthY + lengthX, 2);

    //draw right arm in same manner as the left
    moveto(startX, startY);
    lineto(startX + lengthY, startY + lengthX, 2);
    lineto(startX - lengthX + lengthY, startY + lengthY + lengthX, 2);

    //calculate the x and y components for the golf club drawing.
    //the angle subtraction from the start of this function is undone
    //as the club is at 45 degrees to the arms
    angle += M_PI_4;
    int clubHandleX = startX + lengthY - lengthX;
    int clubHandleY = startY + lengthX + lengthY;

    //change pen color to dark gray and draw the club
    setcolor(DARKGRAY);
    moveto(clubHandleX, clubHandleY);
    lineto(clubHandleX - (int)(27 * cos(angle)), clubHandleY + (int)(27 * sin(angle)), 2);
    filled_circle(clubHandleX - (int)(27 * cos(angle)), clubHandleY + (int)(27 * sin(angle)), 5, DARKGRAY);

    //set pen color back to what it was
    switch(fgColor)
    {
        case 0: setcolor(WHITE);
                break;
        case 1: setcolor(RED);
                break;
        case 2: setcolor(CYAN);
                break;
        case 3: setcolor(YELLOW);
                break;
        default: break;
    }
}

/*Clear the device and redraw the ground, stickman, target and ball at the stickmans feet*/
void redrawEverything(int currentXPos, int resX, int resY, int fgColor, int difficulty)
{
    cleardevice();
    drawStickman(currentXPos, resY, fgColor);
    drawGround(resX, resY);
    drawTarget(resX, resY, difficulty);
    drawBallAtFeet(currentXPos, resY);
}

/*Draw four squares of different colours on screen, and change the background
or foreground colour based on the user selection of one of these squares.
colorToChange indicates whether foreground (0) or background (1) colour is being chosen*/
void changeColors(int colorToChange, int *colorValue, int resX, int resY) //color to change 0 = foreground, 1 = background
{
    //foreground colour values 0 1 2 3, white, red, cyan, yellow respectively
    //background colour values 0 1 2 3, black, blue, lightgray, magenta respectively

    int mouseXPos, mouseYPos;
    //top left xy coordinates of each colour square
    int colorSquareTopLeft[4][2];
    //calculate size of colour squares based on resolution
    int colorSquareSize = resX / 8;

    //write title to display
    cleardevice();
    outtextxy(((resX / 2) - 40), 30, "Change Colour");

    //calculate top left coordinates for color squares
    //[0][0] is square 0 x coord, [0][1] is square 0 y coord
    colorSquareTopLeft[0][0] = (resX / 2) - (2 * colorSquareSize);
    colorSquareTopLeft[1][0] = (resX / 2) + colorSquareSize;
    colorSquareTopLeft[2][0] = colorSquareTopLeft[0][0];
    colorSquareTopLeft[3][0] = colorSquareTopLeft[1][0];
    colorSquareTopLeft[0][1] = 150;
    colorSquareTopLeft[1][1] = colorSquareTopLeft[0][1];
    colorSquareTopLeft[2][1] = ((resY - 150) / 2) + 150;
    colorSquareTopLeft[3][1] = colorSquareTopLeft[2][1];

    //draw colour squares
    //foreground
    if(colorToChange == 0)
    {
        filled_rectangle(colorSquareTopLeft[0][0], colorSquareTopLeft[0][1], colorSquareTopLeft[0][0] + colorSquareSize,
                         colorSquareTopLeft[0][1] + colorSquareSize, WHITE);
        filled_rectangle(colorSquareTopLeft[1][0], colorSquareTopLeft[1][1], colorSquareTopLeft[1][0] + colorSquareSize,
                         colorSquareTopLeft[1][1] + colorSquareSize, RED);
        filled_rectangle(colorSquareTopLeft[2][0], colorSquareTopLeft[2][1], colorSquareTopLeft[2][0] + colorSquareSize,
                         colorSquareTopLeft[2][1] + colorSquareSize, CYAN);
        filled_rectangle(colorSquareTopLeft[3][0], colorSquareTopLeft[3][1], colorSquareTopLeft[3][0] + colorSquareSize,
                         colorSquareTopLeft[3][1] + colorSquareSize, YELLOW);
    }
    //background
    else
    {
        filled_rectangle(colorSquareTopLeft[0][0], colorSquareTopLeft[0][1], colorSquareTopLeft[0][0] + colorSquareSize,
                         colorSquareTopLeft[0][1] + colorSquareSize, BLACK);
        filled_rectangle(colorSquareTopLeft[1][0], colorSquareTopLeft[1][1], colorSquareTopLeft[1][0] + colorSquareSize,
                         colorSquareTopLeft[1][1] + colorSquareSize, BLUE);
        filled_rectangle(colorSquareTopLeft[2][0], colorSquareTopLeft[2][1], colorSquareTopLeft[2][0] + colorSquareSize,
                         colorSquareTopLeft[2][1] + colorSquareSize, LIGHTGRAY);
        filled_rectangle(colorSquareTopLeft[3][0], colorSquareTopLeft[3][1], colorSquareTopLeft[3][0] + colorSquareSize,
                         colorSquareTopLeft[3][1] + colorSquareSize, MAGENTA);

        //draw borders for the coloured squares they dont blend in with the background
        rectangle(colorSquareTopLeft[0][0], colorSquareTopLeft[0][1], colorSquareTopLeft[0][0] + colorSquareSize,
                  colorSquareTopLeft[0][1] + colorSquareSize, 2);
        rectangle(colorSquareTopLeft[1][0], colorSquareTopLeft[1][1], colorSquareTopLeft[1][0] + colorSquareSize,
                  colorSquareTopLeft[1][1] + colorSquareSize, 2);
        rectangle(colorSquareTopLeft[2][0], colorSquareTopLeft[2][1], colorSquareTopLeft[2][0] + colorSquareSize,
                  colorSquareTopLeft[2][1] + colorSquareSize, 2);
        rectangle(colorSquareTopLeft[3][0], colorSquareTopLeft[3][1], colorSquareTopLeft[3][0] + colorSquareSize,
                  colorSquareTopLeft[3][1] + colorSquareSize, 2);
    }

    update_display();

    //wait for the user to click on one of the colours, and set bkcolor or pencolor as necessary
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
            //if mouse is in x range of buttons 0 and 3
            if((mouseXPos >= colorSquareTopLeft[0][0]) && (mouseXPos <= colorSquareTopLeft[0][0] + colorSquareSize))
            {
                //square 1
                if((mouseYPos >= colorSquareTopLeft[0][1]) && (mouseYPos <= colorSquareTopLeft[0][1] + colorSquareSize))
                {
                    if(colorToChange == 0) setcolor(WHITE);
                    else setbkcolor(BLACK);
                    *colorValue = 0;
                    break;
                }
                //square 3
                else if((mouseYPos >= colorSquareTopLeft[2][1]) && (mouseYPos <= colorSquareTopLeft[2][1] + colorSquareSize))
                {
                    if(colorToChange == 0) setcolor(CYAN);
                    else setbkcolor(LIGHTGRAY);
                    *colorValue = 2;
                    break;
                }
            }
            //if mouse is in x range of buttons 2 and 4
            else if((mouseXPos >= colorSquareTopLeft[1][0]) && (mouseXPos <= colorSquareTopLeft[1][0] + colorSquareSize))
            {
                 //square 2
                if((mouseYPos >= colorSquareTopLeft[0][1]) && (mouseYPos <= colorSquareTopLeft[0][1] + colorSquareSize))
                {
                    if(colorToChange == 0) setcolor(RED);
                    else setbkcolor(BLUE);
                    *colorValue = 1;
                    break;
                }
                //square 4
                else if((mouseYPos >= colorSquareTopLeft[2][1]) && (mouseYPos <= colorSquareTopLeft[2][1] + colorSquareSize))
                {
                    if(colorToChange == 0) setcolor(YELLOW);
                    else setbkcolor(MAGENTA);
                    *colorValue = 3;
                    break;
                }
            }
        }
    }
}

/*Calculate the path of the golf ball and animate its motion.
Collisions are checked and a final position is returned when
the ball stops moving or is lost*/
int drawShot(int stickmanXPos, int resX, int resY, float velX, float velY, int bgColor, int fgColor, int level, ObstacleTree tree, int windSpeed, int difficulty)
{
    //setup variables for use in movement calculation
    int initialX = stickmanXPos + 5;//x pos of stickman +5 to put ball in front of stickman
    int initialY = resY - 50 - 5;//resY - 50 is ground level, -5 so ball sits on top of ground
    int posX = initialX, posY = initialY, prevX = initialX, prevY = initialY;
    int falling = 0;//variable to track if the ball is falling or rising - used in collision detection

    int maxHeight = 0, obstacleHit = 0, direction = 1;
    float time = 0;

    //move to balls initial position
    moveto(initialX, initialY);

    //while ball is on screen in x axis, and has not stopped yet
    while(posX <= resX && posX >= 0 && maxHeight != -1)
    {
        maxHeight = posY;
        do
        {
            prevX = posX;
            prevY = posY;

            //calculate y position of ball based on time, acceleration, velocity
            time = (posX - initialX) / velX;
            posY = (int)(initialY - (velY * time) + (GRAVITY * time * time) / 2);

            //draw over old ball in background color
            switch(bgColor)
            {
                case 0: filled_circle(prevX, prevY, 7, BLACK);
                        break;
                case 1: filled_circle(prevX, prevY, 7, BLUE);
                        break;
                case 2: filled_circle(prevX, prevY, 7, LIGHTGRAY);
                        break;
                case 3: filled_circle(prevX, prevY, 7, BROWN);
                        break;
                default: break;
            }

            //draw new ball in white
            filled_circle(posX, posY, 4, WHITE);

            //redraw ground and stickman in case black circle was drawn over them
            drawGround(resX, resY);
            drawTarget(resX, resY, difficulty);
            drawStickman(stickmanXPos, resY, fgColor);
            drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
            drawObstacles(level, resX, resY, tree);
            drawLevelLabels(level, resX, windSpeed);

            //increment or decrement posX depending on the balls direction of travel in x
            if(direction == 1) posX++;
            else posX--;

            //pause for 1 ms before updating to give the illusion of movement
            pausefor(1);
            update_display();

            //track max height (min y value) of current bounce
            if(posY < maxHeight) maxHeight = posY;

            //track vertical direction of the ball
            if(posY <= prevY) falling = 0;
            else falling = 1;

            //check if an obstacle has been hit. if yes, break out of loop
            obstacleHit = checkObstacleHit(resX, resY, posX, posY, level, tree, difficulty, direction, falling);
            if(obstacleHit != 0) break;

        } while(posY <= resY - 54); //resY - 54 (not -50) so ball (radius 4) doesnt go through the ground

        //if bouncing off obstacle (vertical), bounce in the opposite x direction
        if(obstacleHit == 1)
        {
            initialX = posX;
            initialY = posY;
            velX = -0.4 * velX;
            velY = 0.6 * velY;
            obstacleHit = 0;
            direction = abs(direction - 1); //change direction from 0 to 1 or vice versa
        }
        //else if bouncing off top of horizontal obstacle - bounce up with lower velocity
        else if(obstacleHit == 2)
        {
            initialX = posX;
            initialY = posY;
            velY *= 0.6;
            obstacleHit = 0;
        }
        //else if bouncing off bottom of horizontal obstacle, bounce down with lowerer velocity
        else if(obstacleHit == 3)
        {
            initialX = posX;
            initialY = posY;
            velY = -0.6 * velY;
            obstacleHit = 0;
        }
        //else if ball has landed in water - play sinking animation and score 0, exit loop
        else if(obstacleHit == 4)
        {
            //draw over old ball in background color, then play sinking animation
            switch(bgColor)
            {
                case 0: filled_rectangle((resX / 2 - 200), (resY - 50 - 20), (resX / 2 + 100), (resY - 50), BLACK);
                        break;
                case 1: filled_rectangle((resX / 2 - 200), (resY - 50 - 20), (resX / 2 + 100), (resY - 50), BLUE);
                        break;
                case 2: filled_rectangle((resX / 2 - 200), (resY - 50 - 20), (resX / 2 + 100), (resY - 50), LIGHTGRAY);
                        break;
                case 3: filled_rectangle((resX / 2 - 200), (resY - 50 - 20), (resX / 2 + 100), (resY - 50), BROWN);
                        break;
                default: break;
            }
            update_display();
            ballSinkAnimation(posX, resX, resY);
            maxHeight = -1;
        }
        //if ball has fallen in the hole (easy and medium difficulties only)
        else if(obstacleHit == 5)
        {
            //draw over previous ball in background colour
            switch(bgColor)
            {
                case 0: filled_circle(prevX, prevY, 7, BLACK);
                        break;
                case 1: filled_circle(prevX, prevY, 7, BLUE);
                        break;
                case 2: filled_circle(prevX, prevY, 7, LIGHTGRAY);
                        break;
                case 3: filled_circle(prevX, prevY, 7, BROWN);
                        break;
                default: break;
            }
            //redraw target hole and flag in case it was drawn over
            drawTarget(resX, resY, difficulty);
            //draw ball in bottom of hole
            filled_circle(posX, resY - 35, 4, WHITE);
            update_display();
            pausefor(200);

            //returning -1 indicates that the ball is in the hole, rather than at a point on the target used in Hard difficulty
            return -1;
        }
        //else ball bounces off ground - start new arc calculation from bounce point, with lower y velocity
        else
        {
            initialX = posX;
            initialY = resY - 55;
            velY *= 0.6;
            velX *= 0.8;
            obstacleHit = 0;
        }
        //if max height reached by the ball is less than 6 pixels above the ground, count that as stopping
        if(maxHeight > resY - 50 - 6) maxHeight = -1;
    }
    return posX;
}

/*Draw a button on screen in a position specified by the buttons properties*/
void drawButton(MenuButton btn)
{
    //move to top left position of button, draw four sides of box in
    //relation to this point, then write out the label inside the box
    moveto(btn.topLeftX, btn.topLeftY);
    lineto(btn.topLeftX + btn.width, btn.topLeftY,1);
    lineto(btn.topLeftX + btn.width, btn.topLeftY + btn.height,1);
    lineto(btn.topLeftX, btn.topLeftY + btn.height,1);
    lineto(btn.topLeftX, btn.topLeftY,1);
    outtextxy((btn.topLeftX + (btn.width / 4)), (btn.topLeftY + (btn.height / 3)), btn.buttonText);
}

/*Draws power bars on the screen, with the number of bars determined
by the power variable. The size and position of the bars is calculated
based on the y resolution of the graphics window*/
void drawPowerBars(int power, int resY)
{
    int height = (resY - 150)/ 18;
    int topY, bottomY, i;

    //calculate positions for each power bar required for the current
    //value of power, and draw them on screen
    for(i = 0; i < power; i++)
    {
        bottomY = resY - 140 - ((i-1) * height);
        topY = resY - 140 - (i * height);
        //power bar red fill
        filled_rectangle(120, topY, 170, bottomY, RED);
        //power bar border
        rectangle(120, topY, 170, bottomY, 2);
    }
}

/*Draw a small arrow at a specified angle. The base of the arrow
is the centre of the golf ball. Trigonometry is used to calculate
x and y coordinates for each element of the arrow*/
void drawAngleArrow(int stickmanXPos, int resY, float angle)
{
    int lineEndX, lineEndY;

    //draw shaft of arrow length 40
    moveto(stickmanXPos + 5, resY - 50 - 5);
    lineEndX = (stickmanXPos + 5) + (int)(40 * cos(angle));
    lineEndY = (resY - 50 - 5) - (int)(40 * sin(angle));
    lineto(lineEndX, lineEndY, 2);

    //draw head of arrow. each side of the head is at 45 degrees to the shaft and has length 10
    line(lineEndX, lineEndY, (lineEndX - (int)(10 * sin(angle + M_PI_4))), (lineEndY - (int)(10 * cos(angle + M_PI_4))), 2);
    line(lineEndX, lineEndY, (lineEndX + (int)(10 * sin(angle - M_PI_4))), (lineEndY + (int)(10 * cos(angle - M_PI_4))), 2);
}

/*Write out GAME OVER and the players final score, then wait for a click*/
void showEndScreen(int resX, int resY, int score)
{
    char scoreString[5];

    cleardevice();
    sprintf(scoreString, "%d", score);
    outtextxy(resX / 2 - 60, resY / 2 - 100,"GAME OVER");
    outtextxy(resX / 2 - 80, resY / 2 - 60,"Score: ");
    outtextxy(resX / 2, resY / 2 - 60, scoreString);
    outtextxy(resX / 2 - 100, resY / 2 - 20, "Click to continue");
    update_display();
    waitForClick();
}

/*Draw the obstacles required for each level*/
void drawObstacles(int level, int resX, int resY, ObstacleTree tree)
{
    //if level 2 - draw a tree
    if(level == 2)
    {
        filled_rectangle(tree.trunkLeftX, resY - 50 - tree.trunkHeight, tree.trunkLeftX + 50, resY - 50, BROWN); //trunk
        filled_circle(tree.trunkLeftX + 55, resY - 50 - tree.trunkHeight, 40, GREEN); //right circle
        filled_circle(tree.trunkLeftX - 5, resY - 50 - tree.trunkHeight, 40, GREEN); //left circle
        filled_circle(tree.trunkLeftX + 25, resY - 50 - tree.trunkHeight - 30, 40, GREEN); //middle circle
    }
    //if level 3 - draw water
    else if(level == 3)
        filled_rectangle(resX / 2 - 200, resY - 50, resX / 2 + 100, resY, CYAN);//water
}

/*Animate the golf ball sinking below where it landed in the water*/
void ballSinkAnimation(int posX, int resX, int resY)
{
    int i;

    //loop through coordinates from top of water to bottom,
    //so ball appears to sink and move off the bottom of the screen
    for(i = 5; i < 55; i++)
    {
        //redraw the water each time to erase the previous ball
        filled_rectangle(resX / 2 - 200, resY - 50, resX / 2 + 100, resY, CYAN);//water
        filled_circle(posX, resY - 50 + i, 4, WHITE);//golf ball
        update_display();
        pausefor(20);
    }
}

/*Write out the level number and wind speed to the screen*/
void drawLevelLabels(int level, int resX, int windSpeed)
{
    char levelLabelString[15];

    //write level number
    sprintf(levelLabelString, "Level: %d / 3", level);
    outtextxy(resX - 150, 50, levelLabelString);

    //write windspeed and direction - positive wind is left, negative is right
    if(windSpeed > 0)
        sprintf(levelLabelString, "Wind: %d  <-", windSpeed);
    else if(windSpeed < 0)
        sprintf(levelLabelString, "Wind: %d  ->", abs(windSpeed));
    else
        sprintf(levelLabelString, "No Wind");

    outtextxy(resX - 150, 80, levelLabelString);
}
