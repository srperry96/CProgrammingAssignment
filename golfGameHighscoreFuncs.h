#ifndef __GOLFGAMEHIGHSCOREFUNCS_H__
#define __GOLFGAMEHIGHSCOREFUNCS_H__

#include <stdio.h>
#include <string.h>
#include <golfGameGraphicsFuncs.h>

typedef struct
{
    int score;
    char name[10];
} NameAndScore;

void resetHighScores(void);
void readInHighScores(NameAndScore highScoresArray[10]);
void showHighScores(int resX, int resY);
void checkAndSetNewHighScore(int score, int resX, int resY);

#endif //__GOLFGAMEHIGHSCOREFUNCS_H__
