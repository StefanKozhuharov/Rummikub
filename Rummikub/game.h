#pragma once
#include "player.h"
#include "deck.h"

const int MIN_PLAYERS = 2;
const int MAX_PLAYERS = 4;

struct Combination {

	int tiles[DECK_SIZE];
	int count;

};

bool isDigit(char symbol);
int chooseNumberOfPlayers();
bool initializePlayers(Player players[], int numPlayers);
int confirmSelectedTiles(Player& player, int selectedTiles[], int count);
bool readTileSelection(const Player& player, int selectedTiles[], int& count);
int chooseTurnAction();
bool drawTileForPlayer(Player& player);
void playTurn(Player& player, int playerIndex);
void removeSelectedTiles(Player& player, int selectedTiles[], int count);
bool isGameOver(Player players[], int numPlayers, int& winnerIndex);
int calculateHandPoints(const Player& player);
int findWinnerByLowestScore(Player players[], int numPlayers);
bool checkInitial30(Player& player, const int selectedTiles[], int count);
int calculateSelectedTilesPoints(const Player& player, const int selectedTiles[], int count);
bool isValidGroup(const Player& player, const int selectedTiles[], int count);
bool isValidSeries(const Player& player, const int selectedTiles[], int count);
bool isValidMultipleCombination(const Player& player, const Combination combinations[], int combinationCount);
bool readMultipleTileSelection(const Player& player, Combination combinations[], int& combinationCount);