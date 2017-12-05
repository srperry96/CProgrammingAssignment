#include <golfGameHighscoreFuncs.h>
#include <golfGameGraphicsFuncs.h>

//overwrite / create highscores file with default values
void resetHighScores(void)
{
    NameAndScore defaultHighScoresArray[10];

    defaultHighScoresArray[0].score = 100;
    defaultHighScoresArray[1].score = 90;
    defaultHighScoresArray[2].score = 80;
    defaultHighScoresArray[3].score = 70;
    defaultHighScoresArray[4].score = 60;
    defaultHighScoresArray[5].score = 50;
    defaultHighScoresArray[6].score = 40;
    defaultHighScoresArray[7].score = 30;
    defaultHighScoresArray[8].score = 20;
    defaultHighScoresArray[9].score = 10;

    strcpy(defaultHighScoresArray[0].name, "Bella");
    strcpy(defaultHighScoresArray[1].name, "Oli");
    strcpy(defaultHighScoresArray[2].name, "Jack");
    strcpy(defaultHighScoresArray[3].name, "Monty");
    strcpy(defaultHighScoresArray[4].name, "StJohn");
    strcpy(defaultHighScoresArray[5].name, "Dan");
    strcpy(defaultHighScoresArray[6].name, "Edward");
    strcpy(defaultHighScoresArray[7].name, "Sarah");
    strcpy(defaultHighScoresArray[8].name, "Alex");
    strcpy(defaultHighScoresArray[9].name, "Harry");

    FILE *highScoresFile = fopen("highscores.txt", "w");
    int i;
    for(i = 0; i < 10; i++)
    {
        fprintf(highScoresFile, "%d", defaultHighScoresArray[i].score);
        fprintf(highScoresFile, defaultHighScoresArray[i].name);
        fprintf(highScoresFile, "\n");
    }
    fclose(highScoresFile);
}

//read in high scores to array - if file doesnt exist, create one with default high scores
void readInHighScores(NameAndScore highScoresArray[10])
{
    int i, j, flag = 0;
    char nextLetter;

    do
    {
        FILE *highScoresFile = fopen("highscores.txt", "r");
        if(highScoresFile) //if file exists, read scores in to array
        {
            for(i = 0; i < 10; i++)
            {
                fscanf(highScoresFile, "%d", &highScoresArray[i].score);
                j = 0;
                while(1)
                {   //read characters one at a time until a newline char is reached
                    fscanf(highScoresFile, "%c", &nextLetter);
                    if(nextLetter == '\n')
                    { //set terminating character of string at current element
                        highScoresArray[i].name[j] = '\0';
                        break;
                    }
                    else
                    {   //add new char to name string
                        highScoresArray[i].name[j] = nextLetter;
                        j++;
                    }
                }
            }
            fclose(highScoresFile);
            flag = 0;
        }
        else //if file doesnt exist, create it and reset high scores
        {
            resetHighScores();
            printf("\nMISSING HISCORES FILE: SCORES RESET\n");
            flag = 1;
        }
    }while (flag == 1); //If file is missing and has to be created, loop again to read from it
}

void showHighScores(int resX, int resY)
{
    NameAndScore highScoresArray[10];
    int i;
    char scoreString[5];

    readInHighScores(highScoresArray);

    //write out titles
    cleardevice();
    outtextxy(((resX / 2) - 40), 30, "High Scores");
    outtextxy(((resX / 4) - 40), 60, "Name");
    outtextxy(((3 * resX / 4) - 40), 60, "Score");

    //loop through high scores array writing all names and scores to the screen, evenly spaced
    for(i = 0; i < 10; i++)
    {
        //numbers here correspond to those used in writing the titles, so everything is aligned
        outtextxy(((resX / 4) - 40), (100 + (i * ((resY - 100) / 10))), highScoresArray[i].name);
        sprintf(scoreString, "%d", highScoresArray[i].score);
        outtextxy(((3 * resX / 4) - 40), (100 + (i * ((resY - 100) / 10))), scoreString);
    }

    //draw back button
    MenuButton backButton;
    backButton.width = resX / 12;
    backButton.height = resY / 12;
    backButton.topLeftX = 20;
    backButton.topLeftY = 20;
    strcpy(backButton.buttonText, "Back");

    drawButton(backButton);
    update_display();


    int mouseXPos, mouseYPos;
    //wait for back button to be clicked
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
            //if mouse is over back button, break out of loop
            if((mouseXPos >= backButton.topLeftX) && (mouseXPos <= (backButton.topLeftX + backButton.width))
                && (mouseYPos >= backButton.topLeftY) && (mouseYPos <= backButton.topLeftY + backButton.height))
                break;
        }
    }
}

void checkAndSetNewHighScore(int score, int resX, int resY)
{
    NameAndScore highScoresArray[10];
    int i, listPosition = -1;
    char name[10];

    readInHighScores(highScoresArray);

    for(i = 0; i < 10; i++)
    {
        if(score > highScoresArray[i].score)
        {
            listPosition = i;
            break;
        }
    }

    if(listPosition != -1) //if new highscore has been set
    {
        cleardevice();
        outtextxy(resX/2 - 60, resY/12, "NEW HIGHSCORE!");
        outtextxy(resX/2 - 160, 2 * resY/12, "Change to other window to enter your name");
        update_display();

        printf("NEW HIGHSCORE!\nEnter your name: ");
        scanf("%s", name);

        //shift all scores lower than the new high score down one element in the array
        for(i = 9; i > listPosition; i--)
        {
            highScoresArray[i].score = highScoresArray[i-1].score;
            strcpy(highScoresArray[i].name, highScoresArray[i-1].name);
        }
        //save new high score in corresponding element
        highScoresArray[listPosition].score = score;
        strcpy(highScoresArray[listPosition].name, name);

        //open file for writing
        FILE *highScoresFile = fopen("highscores.txt", "w");

        for(i = 0; i < 10; i++)
        {
            fprintf(highScoresFile, "%d", highScoresArray[i].score);
            fprintf(highScoresFile, highScoresArray[i].name);
            fprintf(highScoresFile, "\n");
        }
        fclose(highScoresFile);
    }
}
