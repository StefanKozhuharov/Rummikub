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
int chooseTurnAction(bool didSomething);
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
void sortDescending(int arr[], int n);
void parseTableTilePositions(const char input[], int selectedPos[], int& count, int comboCount);
int confirmStealSelection(const TableCombination combo, const int selectedPos[], int count);
void removeTilesFromTableCombination(TableCombination& combo, int selectedPos[], int count, Tile stolen[], int& stolenCount);
bool addTileCombinationToTable(Table& table, const Tile tiles[], int count);
int stealFromTableAndCreateNewCombination(Player& player, Table& table);
int readNumber(const char input[], int& i);
void printTableCombinationWithPositions(const TableCombination& combo);
bool ensureInitial30(const Player& player);
void printPlayedCombinations(const Player& player, const Combination combos[], int comboCount);
int buildNewComboFromStolenAndHand(const Player& player, const Tile stolen[], int stolenCount, const int handSelected[], int handCount, Tile out[]);
bool validateRemainingTableCombo(const TableCombination& modified);
int readReplacementTileIndex(const Player& player, const bool used[], const char* prompt);
bool tryPlaceStolenTilesOnTable(Table& table, const Tile stolen[], int stolenCount);