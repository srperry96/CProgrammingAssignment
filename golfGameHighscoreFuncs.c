#include <golfGameHighscoreFuncs.h>

/*Read in the high scores from the high scores file.
If the file doesn't exist, create one with the default high scores*/
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
        //if file doesnt exist, create it and reset high scores
        else
        {
            resetHighScores();
            printf("\nMISSING HISCORES FILE: SCORES RESET\n");
            flag = 1;
        }
    }while (flag == 1); //if file was missing and had to be created, program will loop again to read from it
}

/*Display the high scores on screen*/
void showHighScores(int resX, int resY)
{
    NameAndScore highScoresArray[10];
    int i, mouseXPos, mouseYPos;
    char scoreString[5];

    readInHighScores(highScoresArray);

    //write titles on screen
    cleardevice();
    outtextxy(((resX / 2) - 40), 30, "High Scores");
    outtextxy(((resX / 4) - 40), 60, "Name");
    outtextxy(((3 * resX / 4) - 40), 60, "Score");

    //loop through the high scores array writing all names and scores to the screen, evenly spaced on the y axis
    for(i = 0; i < 10; i++)
    {
        //numbers here correspond to those used in writing the titles, so everything is in line
        outtextxy(((resX / 4) - 40), (100 + (i * ((resY - 100) / 10))), highScoresArray[i].name);
        sprintf(scoreString, "%d", highScoresArray[i].score);
        outtextxy(((3 * resX / 4) - 40), (100 + (i * ((resY - 100) / 10))), scoreString);
    }

    //setup and draw the back button in the top left of the screen
    MenuButton backButton;
    backButton.width = resX / 12;
    backButton.height = resY / 12;
    backButton.topLeftX = 20;
    backButton.topLeftY = 20;
    strcpy(backButton.buttonText, "Back");

    drawButton(backButton);
    update_display();

    //wait for the back button to be clicked
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
            //if mouse is over the back button when a click is detected, break out of the loop
            if((mouseXPos >= backButton.topLeftX) && (mouseXPos <= (backButton.topLeftX + backButton.width))
                && (mouseYPos >= backButton.topLeftY) && (mouseYPos <= backButton.topLeftY + backButton.height))
                break;
        }
    }
}

/*Check the players score against the current high scores and,
if necessary, add the new score to the list along with the users name*/
void checkAndSetNewHighScore(int score, int resX, int resY)
{
    NameAndScore highScoresArray[10];
    int i, listPosition = -1;
    char name[10];

    readInHighScores(highScoresArray);

    //loop through highscores to check if player score needs to be added to the high scores list
    for(i = 0; i < 10; i++)
    {
        if(score > highScoresArray[i].score)
        {
            listPosition = i;
            //break out of loop if list position is found, as following loop iterations would be pointless
            break;
        }
    }

     //if a new highscore has been set
    if(listPosition != -1)
    {
        //ask the user for their name
        cleardevice();
        outtextxy(resX/2 - 60, resY/12, "NEW HIGHSCORE!");
        outtextxy(resX/2 - 160, 2 * resY/12, "Change to other window to enter your name");
        update_display();

        printf("NEW HIGHSCORE!\nEnter your name: ");
        scanf("%9s", name); // %9s limits the name length that can be entered to 9 characters (name[] element 10 must hold '\0')

        //clear the input buffer if the user entered too many characters
        int tempChar;
        do tempChar = getchar(); while(tempChar != '\n');

        //shift all scores lower than the new high score down one element in the array
        for(i = 9; i > listPosition; i--)
        {
            highScoresArray[i].score = highScoresArray[i-1].score;
            strcpy(highScoresArray[i].name, highScoresArray[i-1].name);
        }

        //save new high score in corresponding array element
        highScoresArray[listPosition].score = score;
        strcpy(highScoresArray[listPosition].name, name);

        //open high scores file for writing, and write each array element in turn
        FILE *highScoresFile = fopen("highscores.txt", "w");

        for(i = 0; i < 10; i++)
        {
            fprintf(highScoresFile, "%d", highScoresArray[i].score);
            fprintf(highScoresFile, highScoresArray[i].name);
            fprintf(highScoresFile, "\n");
        }
        fclose(highScoresFile);

        printf("Highscore saved! Change to the graphics window to continue");
    }
}

/* Create / overwrite the highscores file with default values*/
void resetHighScores(void)
{
    NameAndScore defaultHighScoresArray[10];

    //initialise default scores and names
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

    //open file and write array elements one by one. each high score and corresponding name has its own line
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
