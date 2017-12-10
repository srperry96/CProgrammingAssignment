#ifndef __GOLFGAMEHIGHSCOREFUNCS_H__
#define __GOLFGAMEHIGHSCOREFUNCS_H__

#include <stdio.h>
#include <string.h>
#include <golfGameGraphicsFuncs.h>

/*Structure for holding a highscore element - a name and a score*/
typedef struct
{
    int score;
    char name[10];
} NameAndScore;

/*Read in the high scores from the high scores file.
If the file doesn't exist, create one with the default high scores*/
void readInHighScores(NameAndScore highScoresArray[10]);

/*Display the high scores on screen*/
void showHighScores(int resX, int resY);

/*Check the players score against the current high scores and,
if necessary, add the new score to the list along with the users name*/
void checkAndSetNewHighScore(int score, int resX, int resY);

/* Create / overwrite the highscores file with default values*/
void resetHighScores(void);

#endif //__GOLFGAMEHIGHSCOREFUNCS_H__
