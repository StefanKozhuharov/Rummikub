#pragma once
#include "deck.h"

const int INITIAL_HAND_SIZE = 14;
const int INITIAL_COMBO_POINTS = 30;

struct Player {

    Tile hand[DECK_SIZE];
    int handCount; //number of tiles in hand
    bool hasInitial30;  //check if initial requirment for 30 points is met

};

void addToHand(Player& player, Tile tile);
void drawPlayerHand(Player& player);