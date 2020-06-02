#ifndef SAVE_H
#define SAVE_H

#include "raylib.h"
#include "constants.h"

typedef enum savePos{
    CHAR0,
    CHAR1,
    CHAR2,
    SCOREPOS
} savePos;

typedef struct saveData{
    char name[3];
    int score;
} saveData;

saveData loadSaveData(int playerID);
void loadScoreboard(saveData scoreboard[]);

void writeSaveData(int playerID, saveData data);
void writeScoreboard(saveData scoreboard[]);

#endif