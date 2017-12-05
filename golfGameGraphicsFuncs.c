#include <golfGameGraphicsFuncs.h>

void drawStickman(int x_position, int y_position)
{
    y_position -= 120;//Y position calc: -70 for height of stickman, -50 for ground height = -120
    circle(x_position, y_position, 10, 2);
    line(x_position, y_position+10, x_position, y_position+40, 2); //body
    line(x_position, y_position+40, x_position - 10, y_position+70, 2);//leftleg
    line(x_position, y_position+40, x_position + 10, y_position+70, 2);//rightleg
    line(x_position - 15, y_position+25, x_position, y_position+25, 2);//left arm - right is drawn with bow elsewhere
}

/*Draw ground line 50 pixels above the bottom of the screen*/
void drawGround(int resX, int resY)
{
    line(0, resY-50, resX, resY-50, 2);
}

void drawTarget(int resX, int resY)
{
    int scores[5] = {10, 20, 30, 20, 10};
    char strScores[3];
    int i, pos;
    //draw lines and labels for target
    for(i = 0; i < 5; i++)
    {
        pos = (i*30) + resX - 200;
        moveto(pos, resY-50);
        lineto(pos, resY-40, 1);
        sprintf(strScores, "%d", scores[i]);
        outtextxy(pos+6, resY-45, strScores);
    }

    //draw last line of target (right most line)
    pos = (5*30) + resX - 200;
    moveto(pos, resY-50);
    lineto(pos, resY-40, 1);
}

void drawBow(int startX, int resY, double power, float angle)
{
    //lengths of arm in x and y for specific angle
    int lengthX = (int)(20 * cos(angle));
    int lengthY = (int)(20 * sin(angle));

    int startY = resY - 120 + 25; //same calculation as in drawStickman function

    printf("\nX:%d, %d; Y:%d, %d; P:%0.2f\n", startX, lengthX, startY, lengthY, power); //debugging

    int angleInDegs = (int)(angle*(180/M_PI)); //convert to degrees
    line(startX, startY, startX + lengthX, startY - lengthY, 2);
    arc(startX, startY, 20, -angleInDegs - 60, 120, 1);//bow

    update_display();
}

void redrawEverything(int currentXPos, int resX, int resY) //redraw ground, target, stickman
{
    cleardevice();
    drawStickman(currentXPos, resY);
    drawGround(resX, resY);
    drawTarget(resX, resY);
    update_display();
}

void getColors(void)
{
    bool flag = false;
    char colorLetter;

    //choose background colour: black or white
    do{
        printf("Choose a colour for the background: (w)hite (b)lack\n");
        scanf(" %c", &colorLetter);

        switch(colorLetter)
        {
            case 'W':
            case 'w': setbkcolor(WHITE);
                      flag = false;
                      break;
            case 'B':
            case 'b': setbkcolor(BLACK);
                      flag = false;
                      break;
            default: printf("Invalid\n");
                     flag = true;
                     break;
        }
    }while(flag == true);

    flag = false; //reset flag

    //choose stickman colour: red, green, blue
    do{
        printf("Choose a colour for the stickman: r g b\n");
        scanf(" %c", &colorLetter);

        switch(colorLetter)
        {
            case 'R':
            case 'r': setcolor(RED);
                      flag = false;
                      break;
            case 'G':
            case 'g': setcolor(GREEN);
                      flag = false;
                      break;
            case 'B':
            case 'b': setcolor(BLUE);
                      flag = false;
                      break;
            default: printf("Invalid\n");
                     flag = true;
                     break;
        }
    }while(flag == true);
}
//OLD DRAW ARC CODE
//int drawArc(int stickmanXPos, int resX, int resY, float velX, float velY)
//{
//    int initialX = (stickmanXPos + 15);//x pos of stickman + 15 for length of arm
//    int initialY = (resY - 95);//-120 to get stickman position + 25 to get arm position = -95
//    int posX = initialX;
//    int posY = initialY;
//    float gravity = 9.81, time = 0;
//
//    moveto(initialX, initialY);
//
//    do
//    {
//        time = (posX - initialX) / velX;
//        posY = (int)(initialY - (velY * time) + (gravity*time*time)/2);
//        lineto(posX, posY, 1);
//        posX++;
//        pausefor(1); //wait 1ms
//        update_display();//having update display here makes arc look like something being thrown
//
//
//    } while( (posX <= resX) && (posY <= resY-51) ); //resY-51 not -50 so arc doesnt go through the ground
//
//    posX--; //undo posX++ from final loop through as it happens after drawing the arc
//    return posX;
//}

int drawArc(int stickmanXPos, int resX, int resY, float velX, float velY)
{
    int initialX = (stickmanXPos + 15);//x pos of stickman + 15 for length of arm
    int initialY = (resY - 95);//-120 to get stickman position + 25 to get arm position = -95
    int posX = initialX;
    int posY = initialY;
    float gravity = 9.81, time = 0;
    int maxHeight = 0;
    int obstacleHit = 0, direction = 1;

    moveto(initialX, initialY);

while(posX <= resX && maxHeight != -1)
{
    maxHeight = posY;
    do
    {
        time = (posX - initialX) / velX;
        posY = (int)(initialY - (velY * time) + (gravity*time*time)/2);
        lineto(posX, posY, 1);
        if(direction == 1) posX++;
        else posX--;

        pausefor(1); //wait 1ms
        update_display();//having update display here makes arc look like something being thrown

        if(posY < maxHeight) maxHeight = posY; //log max height (min y value) of current bounce

        if(posX > resX - 20)
        {
            obstacleHit = 1;
            break;
        }
    } while(posY <= resY-51); //resY-51 not -50 so arc doesnt go through the ground

    //if bouncing off obstacle not ground
    if(obstacleHit == 1)
    {
        initialX = posX;
        initialY = posY;
        velX = -0.6 * velX;
        velY = 0.6 * velY;
        obstacleHit = 0;
        direction = abs(direction - 1); //change direction from 0 to 1 or vice versa
    }
    else //if bouncing off ground
    {
        initialY = resY - 52;
        velY *= 0.6;
        initialX = posX;
    }
    if(maxHeight > resY - 50 - 10) maxHeight = -1; //if max height reached by the ball is less than 10 pixels above the ground, count that as stopping
}

    posX--; //undo posX++ from final loop through as it happens after drawing the arc
    return posX;
}

void drawPowerArrow(int mouseOldX, int mouseNewX, int mouseOldY, int mouseNewY, double power, float angle)
{
    char angleLabel[5];
    char powerLabel[7];
    float angleInDegs;

    //labels for angle and power
    angleInDegs = angle*(180/M_PI); //convert to degrees for output
    printf("angle is %f\n", angleInDegs);
    sprintf(angleLabel, "%0.0f", angleInDegs);
    sprintf(powerLabel, "%0.0f", power);
    outtextxy(mouseOldX+10, mouseOldY+20, powerLabel);
    outtextxy(mouseOldX+10, mouseOldY-10, angleLabel);

    //drawing the arrow =================only works pointing up and right, between 0 and 90
    float headAngle;

    headAngle = angle + M_PI_4;
    line(mouseOldX, mouseOldY, (mouseOldX - (power/5)*sin(headAngle)), (mouseOldY - (power/5)*cos(headAngle)), (power/20) );
    headAngle = angle - M_PI_4;
    line(mouseOldX, mouseOldY, (mouseOldX + (power/5)*sin(headAngle)), (mouseOldY + (power/5)*cos(headAngle)), (power/20) );

    moveto(mouseOldX, mouseOldY);
    lineto(mouseNewX, mouseNewY, (power/20) );
}

void drawButton(MenuButton btn)
{
    moveto(btn.topLeftX, btn.topLeftY);
    lineto(btn.topLeftX + btn.width, btn.topLeftY,1);
    lineto(btn.topLeftX + btn.width, btn.topLeftY + btn.height,1);
    lineto(btn.topLeftX, btn.topLeftY + btn.height,1);
    lineto(btn.topLeftX, btn.topLeftY,1);
    outtextxy((btn.topLeftX + (btn.width / 5)), (btn.topLeftY + (btn.height / 3)), btn.buttonText);
}
