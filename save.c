#include "save.h"

saveData loadSaveData(int playerID){
    saveData read;
    read.name[0] = (char) LoadStorageValue(playerID + CHAR0);
    read.name[1] = (char) LoadStorageValue(playerID + CHAR1);
    read.name[2] = (char) LoadStorageValue(playerID + CHAR1);
    read.score = LoadStorageValue(playerID + SCOREPOS);

    return read;
}

void loadScoreboard(saveData scoreboard[]){
    for (int i = 0; i < scoreboardSize; i++){
        scoreboard[i] = loadSaveData(i);
    }
}

void writeSaveData(int playerID, saveData data){
    SaveStorageValue(playerID + CHAR0, (int) data.name[0]);
    SaveStorageValue(playerID + CHAR1, (int) data.name[1]);
    SaveStorageValue(playerID + CHAR2, (int) data.name[2]);
    SaveStorageValue(playerID + SCOREPOS, data.score);
}

void writeScoreboard(saveData scoreboard[]){
    for (int i = 0; i < scoreboardSize; i++){
        writeSaveData(i, scoreboard[i]);
    }
}