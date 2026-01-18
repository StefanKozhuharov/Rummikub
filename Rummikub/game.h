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

enum TurnResult {

	TURN_CONTINUE,
	TURN_DID_SOMETHING,
	TURN_END,
	TURN_DRAW_END

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
bool hasAnyValidJokerReplacement(const Player& player, const TableCombination& modified, int jokerPos, const bool usedHand[]);
void sortAsc(int arr[], int n);
bool buildSeriesData(const Tile tiles[], int count, int values[], int& valueCount, int& jokers, Colour& colour);
bool isValidSeriesCore(const Tile tiles[], int count);
TurnResult handleAddToTable(Player& player, Table& table);
TurnResult handleSteal(Player& player, Table& table);
TurnResult handlePlayNewCombinations(Player& player, Table& table);
int readStealPositionsUI(const TableCombination& original, int selectedPos[], int& posCount);
int applyStealToCombination(Player& player, const TableCombination& original, const int selectedPos[], int posCount, TableCombination& modified, Tile stolen[], int& stolenCount, int replaceHandIndex[], int& replaceCount, bool usedHand[], bool& didSplit, TableCombination& splitSecond);
char chooseStealMode();
void removeUniqueHandIndexes(Player& player, const int index[], int n);
int handleModeAddToTable(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile stolen[], int stolenCount, const int replaceHandIndex[], int replaceCount);
int handleModeCreateNewCombo(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile stolen[], int stolenCount, const int replaceHandIndex[], int replaceCount, const bool usedHand[]);
bool trySplitSeriesIntoTwoValid(const TableCombination& in, TableCombination& outA, TableCombination& outB);
bool trySplitRemainingCombo(const TableCombination& in, TableCombination& outA, TableCombination& outB);
bool isRemainingTableComboOk(const TableCombination& modified);
bool stealSinglePos(Player& player, const TableCombination& original, int pos, TableCombination& modified, Tile stolen[], int& stolenCount, int replaceHandIndex[], int& replaceCount, bool usedHand[], int removePos[], int& removeCount);
void removePositionsFromModified(TableCombination& modified, int removePos[], int removeCount);
int finalizeStealWithSplitIfNeeded(TableCombination& modified, bool& didSplit, TableCombination& splitSecond);
int filterUnusedHandSelection(const Player& player, int handSelected[], int handCount, const bool usedHand[]);
bool prepareNewComboTiles(const Player& player, const Tile stolen[], int stolenCount, int handSelected[], int& handCount, const bool usedHand[], Tile newComboTiles[], int& newCount);
int buildAllToRemove(const int replaceHandIndex[], int replaceCount, const int handSelected[], int handCount, int allToRemove[]);
bool commitStealModeCreateNewCombo(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile newComboTiles[], int newCount, const int allToRemove[], int allCount);
bool collectSeriesSplitData(const TableCombination& in, Tile nonJokers[], int& nonCount, Tile jokers[], int& jokerCount, int values[], int& valueCount, Colour& colour);
void buildSortedNonJokers(const Tile nonJokers[], int nonCount, int values[], int valueCount, Tile sortedNon[]);
bool tryAllCutsAndJokers(const Tile sortedNon[], int nonCount, const Tile jokers[], int jokerCount, TableCombination& outA, TableCombination& outB);
bool chooseStealSource(const Table& table, int& tableIndex, TableCombination& original);
int readStealPositions(const TableCombination& original, int selectedPos[], int& posCount);
int buildStealResult(Player& player, const TableCombination& original, const int selectedPos[], int posCount, TableCombination& modified, Tile stolen[], int& stolenCount, int replaceHandIndex[], int& replaceCount, bool usedHand[], bool& didSplit, TableCombination& splitSecond);
int runStealModeLoop(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile stolen[], int stolenCount, const int replaceHandIndex[], int replaceCount, const bool usedHand[]);