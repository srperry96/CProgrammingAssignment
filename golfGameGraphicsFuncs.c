#include <golfGameGraphicsFuncs.h>
#include <golfGamePlayFuncs.h>

void drawStickman(int x_position, int y_position, int fgColor)
{
    y_position -= 120;//Y position calc: -70 for height of stickman, -50 for ground height = -120
    //draw head in correct color
    switch(fgColor)
    {
        case 0: filled_circle(x_position, y_position, 10, WHITE);
                break;
        case 1: filled_circle(x_position, y_position, 10, RED);
                break;
        case 2: filled_circle(x_position, y_position, 10, CYAN);
                break;
        case 3: filled_circle(x_position, y_position, 10, YELLOW);
                break;
        default: break;
    }

    line(x_position, y_position+10, x_position, y_position+40, 2); //body
    line(x_position, y_position+40, x_position - 10, y_position+70, 2);//leftleg
    line(x_position, y_position+40, x_position + 10, y_position+70, 2);//rightleg
}

/*Draw ground line 50 pixels above the bottom of the screen*/
void drawGround(int resX, int resY)
{
    //line(0, resY-50, resX, resY-50, 2);
    filled_rectangle(0, resY-50, resX, resY, GREEN);
}

void drawBallAtFeet(int stickmanXPos, int resY)
{
    filled_circle(stickmanXPos + 5, resY - 50 - 5, 4, WHITE);
}

void drawTarget(int resX, int resY)
{
    int scores[5] = {10, 20, 30, 20, 10};
    char strScores[3];
    int i, pos;
    //draw lines and labels for target
    for(i = 0; i < 5; i++)
    {
        pos = (i * 30) + resX - 200;
        moveto(pos, resY - 50);
        lineto(pos, resY - 40, 1);
        sprintf(strScores, "%d", scores[i]);
        outtextxy(pos + 6, resY - 45, strScores);
    }

    //draw last line of target (right most line)
    pos = (5 * 30) + resX - 200;
    moveto(pos, resY - 50);
    lineto(pos, resY - 40, 1);
}

void drawArmsAndClub(int startX, int resY, float angle, int fgColor)
{
    angle -= M_PI_4;
    //lengths of arm in x and y for specific angle
    int lengthX = (int)(20 * cos(angle));
    int lengthY = (int)(20 * sin(angle));

    int startY = resY - 120 + 15; //same calculation as in drawStickman function

    int lengthX2 = (int)(20 * sin(angle));
    int lengthY2 = (int)(20 * cos(angle));
  //left arm
    moveto(startX, startY);
    lineto(startX - lengthX, startY + lengthY, 2);
    lineto(startX - lengthX + lengthX2, startY + lengthY + lengthY2, 2);
//right arm
    moveto(startX, startY);
    lineto(startX + lengthY, startY + lengthX, 2);
    lineto(startX - lengthX + lengthX2, startY + lengthY + lengthY2, 2);
//club
    angle += M_PI_4;
    int clubHandleX = (startX + lengthY) - lengthX;
    int clubHandleY = (startY + lengthX + lengthY);

    //change pen color to dark gray and draw club
    setcolor(DARKGRAY);
    moveto(clubHandleX, clubHandleY);
    lineto(clubHandleX - (int)(27 * cos(angle)), clubHandleY + (int)(27 * sin(angle)), 2);
    filled_circle(clubHandleX - (int)(27 * cos(angle)), clubHandleY + (int)(27 * sin(angle)), 5, DARKGRAY);
    //set pen color back to what it should be
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

void redrawEverything(int currentXPos, int resX, int resY, int fgColor) //redraw ground, target, stickman, ball
{
    cleardevice();
    drawStickman(currentXPos, resY, fgColor);
    drawGround(resX, resY);
    drawTarget(resX, resY);
    drawBallAtFeet(currentXPos, resY);
}

void changeColors(int colorToChange, int *colorValue, int resX, int resY) //color to change 0 = foreground, 1 = background
{
//foreground 0 1 2 3, white, red, cyan, yellow respectively
//background 0 1 2 3, black, blue, lightgray, magenta respectively

    int mouseXPos, mouseYPos;
    int colorSquareTopLeft[4][2]; //top left xy coordinates of each colour square
    int colorSquareSize = resX / 8; //size of color squares scales with resolution

    cleardevice();
    outtextxy(((resX / 2) - 40), 30, "Change Colour");

    //calculate top left coordinate for color squares
    colorSquareTopLeft[0][0] = (resX / 2) - (2 * colorSquareSize);//x coord colour 1
    colorSquareTopLeft[1][0] = (resX / 2) + colorSquareSize;
    colorSquareTopLeft[2][0] = colorSquareTopLeft[0][0];
    colorSquareTopLeft[3][0] = colorSquareTopLeft[1][0];

    colorSquareTopLeft[0][1] = 150;
    colorSquareTopLeft[1][1] = colorSquareTopLeft[0][1];
    colorSquareTopLeft[2][1] = ((resY - 150) / 2) + 150;
    colorSquareTopLeft[3][1] = colorSquareTopLeft[2][1];

    //draw color squares
    if(colorToChange == 0) //foreground
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
    else //background
    {
        filled_rectangle(colorSquareTopLeft[0][0], colorSquareTopLeft[0][1], colorSquareTopLeft[0][0] + colorSquareSize,
                         colorSquareTopLeft[0][1] + colorSquareSize, BLACK);
        filled_rectangle(colorSquareTopLeft[1][0], colorSquareTopLeft[1][1], colorSquareTopLeft[1][0] + colorSquareSize,
                         colorSquareTopLeft[1][1] + colorSquareSize, BLUE);
        filled_rectangle(colorSquareTopLeft[2][0], colorSquareTopLeft[2][1], colorSquareTopLeft[2][0] + colorSquareSize,
                         colorSquareTopLeft[2][1] + colorSquareSize, LIGHTGRAY);
        filled_rectangle(colorSquareTopLeft[3][0], colorSquareTopLeft[3][1], colorSquareTopLeft[3][0] + colorSquareSize,
                         colorSquareTopLeft[3][1] + colorSquareSize, MAGENTA);
        //draw borders for squares so background doesnt blend with color square
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
            //mouse is in x range of buttons 0 and 3
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
            } //mouse is in x range of buttons 2 and 4
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

int drawShot(int stickmanXPos, int resX, int resY, float velX, float velY, int bgColor, int fgColor, int level, ObstacleTree tree, int windSpeed)
{
    int initialX = stickmanXPos + 5;//x pos of stickman + extra to put ball in front of stickman
    int initialY = resY - 50 - 5;//resY - 50 is ground level, - 10 so ball sits on top of ground
    int posX = initialX;
    int posY = initialY;
    int prevX = initialX, prevY = initialY;
    int maxHeight = 0, obstacleHit = 0, direction = 1;
    float gravity = 9.81, time = 0;

    moveto(initialX, initialY);

    while(posX <= resX && posX >= 0 && maxHeight != -1 ) //while ball is on screen in x axis, and not stopped
    {
        maxHeight = posY;
        do
        {
            prevX = posX;
            prevY = posY;

            time = (posX - initialX) / velX;
            posY = (int)(initialY - (velY * time) + (gravity * time * time) / 2);

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
            drawTarget(resX, resY);
            drawStickman(stickmanXPos, resY, fgColor);
            drawArmsAndClub(stickmanXPos, resY, M_PI_2, fgColor);
            drawObstacles(level, resX, resY, tree);
            drawLevelLabels(level, resX, windSpeed);

            if(direction == 1) posX++;
            else posX--;

            pausefor(1); //wait 1ms
            update_display();//update display here makes ball look like its moving

            if(posY < maxHeight) maxHeight = posY; //track max height (min y value) of current bounce

            //check if an obstacle has been hit. if yes, break out of loop
            obstacleHit = checkObstacleHit(resX, resY, posX, posY, level, tree);
            if(obstacleHit != 0) break;

        } while(posY <= resY - 54); //resY - 54 (not -50) so ball (radius 4) doesnt go through the ground

        //if bouncing off obstacle vertical
        if(obstacleHit == 1)
        {
            initialX = posX;
            initialY = posY;
            velX = -0.6 * velX;
            velY = 0.6 * velY;
            obstacleHit = 0;
            direction = abs(direction - 1); //change direction from 0 to 1 or vice versa
        }
        //else if bouncing off top of obstacle horizontal
        else if(obstacleHit == 2)
        {
            initialX = posX;
            initialY = posY;
            velY *= 0.6;
            obstacleHit = 0;
        }
        //else if bouncing off bottom of obstacle horizontal
        else if(obstacleHit == 3)
        {
            initialX = posX;
            initialY = posY;
            velY = -0.6 * velY;
            obstacleHit = 0;
        }
        //else if ball has landed in water
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
        //else ball bounces off ground
        else
        {
            initialX = posX;
            initialY = resY - 55;
            velY *= 0.6;
            obstacleHit = 0;
        }
        //if max height reached by the ball is less than 6 pixels above the ground, count that as stopping
        if(maxHeight > resY - 50 - 6) maxHeight = -1;
    }
    return posX;
}

void drawButton(MenuButton btn)
{
    moveto(btn.topLeftX, btn.topLeftY);
    lineto(btn.topLeftX + btn.width, btn.topLeftY,1);
    lineto(btn.topLeftX + btn.width, btn.topLeftY + btn.height,1);
    lineto(btn.topLeftX, btn.topLeftY + btn.height,1);
    lineto(btn.topLeftX, btn.topLeftY,1);
    outtextxy((btn.topLeftX + (btn.width / 4)), (btn.topLeftY + (btn.height / 3)), btn.buttonText);
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

void drawObstacles(int level, int resX, int resY, ObstacleTree tree)
{
    if(level == 2)
    {
        filled_rectangle(tree.trunkLeftX, resY - 50 - tree.trunkHeight, tree.trunkLeftX + 50, resY - 50, BROWN); //trunk
        filled_circle(tree.trunkLeftX + 55, resY - 50 - tree.trunkHeight, 40, GREEN); //right circle
        filled_circle(tree.trunkLeftX - 5, resY - 50 - tree.trunkHeight, 40, GREEN); //left circle
        filled_circle(tree.trunkLeftX + 25, resY - 50 - tree.trunkHeight - 30, 40, GREEN); //middle circle
    }
    else if(level == 3)
        filled_rectangle(resX / 2 - 200, resY - 50, resX / 2 + 100, resY, CYAN);//water
}

void ballSinkAnimation(int posX, int resX, int resY)
{
    int i;
    for(i = 5; i < 55; i++)
    {
        filled_rectangle(resX / 2 - 200, resY - 50, resX / 2 + 100, resY, CYAN);//water
        filled_circle(posX, resY - 50 + i, 4, WHITE);
        update_display();
        pausefor(20);
    }
}

void drawLevelLabels(int level, int resX, int windSpeed)
{
    char levelLabelString[15];

    //write level number
    sprintf(levelLabelString, "Level: %d / 3", level);
    outtextxy(resX - 150, 50, levelLabelString);

    //write windspeed and direction
    if(windSpeed > 0)
        sprintf(levelLabelString, "Wind: %d  <-", windSpeed);
    else if(windSpeed < 0)
        sprintf(levelLabelString, "Wind: %d  ->", abs(windSpeed));
    else
        sprintf(levelLabelString, "No Wind");

    outtextxy(resX - 150, 80, levelLabelString);
}
