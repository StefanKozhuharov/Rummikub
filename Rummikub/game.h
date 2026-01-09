#pragma once
#include "player.h"
#include "deck.h"

const int MIN_PLAYERS = 2;
const int MAX_PLAYERS = 4;

bool isDigit(char symbol);
int chooseNumberOfPlayers();
bool initializePlayers(Player players[], int numPlayers);
bool confirmSelectedTiles(Player player, int selectedTiles[], int count);
void readTileSelection(const Player& player, int selectedTiles[], int& count);