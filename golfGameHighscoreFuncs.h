#ifndef __GOLFGAMEHIGHSCOREFUNCS_H__
#define __GOLFGAMEHIGHSCOREFUNCS_H__

#include <stdio.h>
#include <string.h>

typedef struct
{
    int score;
    char name[10];
} NameAndScore;

void resetHighScores(void);
void readInHighScores(NameAndScore highScoresArray[10]);
void showHighScores(void);
void checkAndSetNewHighScore(int score);

#endif //__GOLFGAMEHIGHSCOREFUNCS_H__
