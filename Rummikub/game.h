#pragma once
#include "player.h"
#include "deck.h"
#include "table.h"

const int MIN_PLAYERS = 2;
const int MAX_PLAYERS = 4;

struct Combination {

	int tiles[DECK_SIZE];
	int count;

};

bool isDigit(char symbol);
int chooseNumberOfPlayers();
bool initializePlayers(Player players[], int numPlayers);
int chooseTurnAction();
bool drawTileForPlayer(Player& player);
void playTurn(Player& player, int playerIndex, Table& table);
void removeSelectedTiles(Player& player, int selectedTiles[], int count);
bool isGameOver(Player players[], int numPlayers, int& winnerIndex);
int calculateHandPoints(const Player& player);
int findWinnerByLowestScore(Player players[], int numPlayers);
int calculateSelectedTilesPoints(const Player& player, const int selectedTiles[], int count);
bool isValidGroup(const Player& player, const int selectedTiles[], int count);
bool isValidSeries(const Player& player, const int selectedTiles[], int count);
bool isValidMultipleCombination(const Player& player, const Combination combinations[], int combinationCount);
bool readMultipleTileSelection(const Player& player, Combination combinations[], int& combinationCount);
int calculateMultipleSelectedTilesPoints(const Player& player, const Combination combinations[], int combinationCount);
bool checkInitial30Multiple(Player& player, const Combination combinations[], int combinationCount);
int confirmSelectedCombinations(const Player& player, const Combination combinations[], int combinationCount);
void parseMultipleTileSelection(const Player& player, const char input[], Combination combinations[], int& combinationCount);
void removeMultipleSelectedTiles(Player& player, const Combination combinations[], int combinationCount);
bool addCombinationsToTable(const Player& player, Table& table, const Combination combinations[], int combinationCount);
int chooseTableCombinationIndex(const Table& table);
bool isValidGroupTiles(const Tile tiles[], int count);
bool isValidSeriesTiles(const Tile tiles[], int count);
bool isValidCombinationTiles(const Tile tiles[], int count);
bool readSingleSelectionOrDraw(const Player& player, int selectedTiles[], int& count);
int addTilesToTableCombination(Player& player, Table& table);
void parseSingleTileSelection(const Player& player, const char input[], int selectedTiles[], int& count);
int confirmSelectedTilesSingle(const Player& player, const int selectedTiles[], int count);