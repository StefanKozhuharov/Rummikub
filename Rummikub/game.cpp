/**
*
* Solution to course project #07
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Stefan Kozhuharov
* @idnumber 9MIO600529
* @compiler VC
*
* <containts all functions and structures needed for the game loop>
*
*/
#include "game.h"
#include "ui.h"
#include <iostream>

using namespace std;

// checks if a given character is a decimal digit
bool isDigit(char symbol) {

	return symbol >= '0' && symbol <= '9';

}

void sortDescending(int arr[], int n) {

	for (int i = 0; i < n - 1; i++) {

		for (int j = i + 1; j < n; j++) {

			if (arr[i] < arr[j]) {

				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;

			}

		}

	}

}

// parses a positive integer from an input string
// used by input parsing functions for reading tile positions
int readNumber(const char input[], int& i) {

	int number = 0;
	while (isDigit(input[i])) {

		number = number * 10 + (input[i] - '0');
		i++;

	}

	return number;

}

void sortAsc(int arr[], int n) {

	for (int i = 0; i < n - 1; i++) {

		for (int j = i + 1; j < n; j++) {

			if (arr[i] > arr[j]) {

				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;

			}
		}

	}

}

// prints a table combination's positions
void printTableCombinationWithPositions(const TableCombination& combo) {

	for (int i = 0; i < combo.count; i++) {

		cout << "[" << i + 1 << "]";
		printTile(combo.tiles[i]);
		cout << " ";

	}

	cout << endl;

}

// reads and validates the number of players (2 to 4 are allowed)
int chooseNumberOfPlayers() {

	int numPlayers = 0;

	while (numPlayers < 2 || numPlayers > 4) {

		cout << "Enter number of players (2-4): ";
		cin >> numPlayers;
		cin.ignore(1024, '\n');

	}

	return numPlayers;

}

// initializes the players and draws their hand of 14 tiles for each
// returns false if deck doesn't have enough tiles (shouldn't be possible)
bool initializePlayers(Player players[], int numPlayers) {

	for (int i = 0; i < numPlayers; i++) {

		players[i].handCount = 0;
		players[i].hasInitial30 = false;

		if (!drawPlayerHand(players[i])) {

			cout << "Not enough tiles to draw initial hand for player " << i + 1 << "!" << endl;
			return false;

		}

	}

	return true;

}

// prints a message to inform the player what actions they can make
// if the player has done something they can end their turn without having to draw
int chooseTurnAction(bool didSomething) {

	int choice = 0;

	while (true) {

		cout << "Choose Action" << endl << "1 - Play new combinations" << endl << "2 - Add tile(s) to existing table combination" << endl << "3 - Draw a tile (ends turn)" << endl << "4 - Steal tile(s) from the table (you need to use it on the same turn)" << endl;

		if (didSomething) {

			cout << "5 - End turn" << endl;

		}

		cin >> choice;
		cin.ignore(1024, '\n');

		if (!didSomething) {

			if (choice >= 1 && choice <= 4) {

				return choice;

			}

			cout << "Invalid choice!" << endl;

		}
		else {

			if (choice >= 1 && choice <= 5) {

				return choice;

			}

			cout << "Invalid choice!" << endl;

		}

	}

}

// draws one tile from the deck and adds it to the player's hand
// the drawn tile can't be accessed again by reducing the remaining tiles value
bool drawTileForPlayer(Player& player) {

	Tile tile;

	if (!drawTile(tile)) {

		cout << "No tiles left in the deck!" << endl;
		return false;

	}

	if (!addToHand(player, tile)) {

		cout << "Your hand is full!" << endl;
		return false;

	}

	cout << "You drew: ";
	printTile(tile);
	cout << endl;

	return true;

}

// main turn loop for a single player
// keeps asking the player to do something until he draws a tile or ends their turn after another action (apart from drawing)
void playTurn(Player& player, int playerIndex, Table& table) {

	cout << endl << "--- Player " << playerIndex + 1 << "'s turn ---" << endl;

	bool didSomething = false;

	while (true) {

		printTable(table);
		printHand(player);

		int action = chooseTurnAction(didSomething);

		TurnResult r = TURN_CONTINUE;

		if (action == 1) {

			r = handlePlayNewCombinations(player, table);

		}
		else if (action == 2) {

			r = handleAddToTable(player, table);

		}
		else if (action == 3) {

			r = TURN_DRAW_END;

		}
		else if (action == 4) {

			r = handleSteal(player, table);

		}
		else if (action == 5) {

			r = TURN_END;

		}

		if (r == TURN_DRAW_END) {

			drawTileForPlayer(player);
			return;

		}
		if (r == TURN_END) {

			return;

		}
		if (r == TURN_DID_SOMETHING) {

			didSomething = true;

		}

	}

}

// removes the tiles selected by the player to place on the table from their hand
// indexes are sorted in a descending order first to avoid shufling issues
void removeSelectedTiles(Player& player, int selectedTiles[], int count) {

	sortDescending(selectedTiles, count);

	for (int i = 0; i < count; i++) {

		int index = selectedTiles[i];

		for (int j = index; j < player.handCount - 1; j++) {

			player.hand[j] = player.hand[j + 1];

		}

		player.handCount--;

	}

}

// checks if the game ending conditions are met
// 1. a player's hand is empty
// 2 the deck is empty -> winner is decided by who has the least ammount of points
bool isGameOver(Player players[], int numPlayers, int& winnerIndex) {

	for (int i = 0; i < numPlayers; i++) {

		if (players[i].handCount == 0) {

			winnerIndex = i;
			return true;

		}

	}

	if (remainingTiles == 0) {

		winnerIndex = findWinnerByLowestScore(players, numPlayers);
		return true;

	}

	return false;

}

// calculates the total point value of a player's hand
int calculateHandPoints(const Player& player) {

	int sum = 0;

	for (int i = 0; i < player.handCount; i++) {

		if (player.hand[i].value == JOKER_VALUE) {

			sum += 30;

		}
		else {

			sum += player.hand[i].value;

		}

	}

	return sum;

}

// chooses the winner based on score
int findWinnerByLowestScore(Player players[], int numPlayers) {

	int winnerIndex = 0;
	int lowestScore = calculateHandPoints(players[0]);

	for (int i = 1; i < numPlayers; i++) {

		int currentScore = calculateHandPoints(players[i]);

		if (currentScore < lowestScore) {

			lowestScore = currentScore;
			winnerIndex = i;

		}

	}

	return winnerIndex;

}

// calculates the point value of the selected tiles during a player's turn
// used for initial 30 rule
int calculateSelectedTilesPoints(const Player& player, const int selectedTiles[], int count) {

	int sum = 0;

	for (int i = 0; i < count; i++) {

		int index = selectedTiles[i];
		int value = player.hand[index].value;

		if (value == JOKER_VALUE) {

			sum += 30;

		}
		else {

			sum += value;

		}

	}

	return sum;

}

// validates a group combination
// these are 3-4 tiles of the same value, all different colours
bool isValidGroup(const Player& player, const int selectedTiles[], int count) {

	if (count < 3 || count > 4) {

		return false;

	}

	bool usedColours[4] = { false, false, false, false };
	int jokers = 0;
	int value = -1;

	for (int i = 0; i < count; i++) {

		Tile tile = player.hand[selectedTiles[i]];

		if (tile.value == JOKER_VALUE) {

			jokers++;
			continue;

		}

		if (value == -1) {

			value = tile.value;

		}
		else if (tile.value != value) {

			return false;

		}

		if (usedColours[tile.colour]) {

			return false;

		}

		usedColours[tile.colour] = true;

	}

	int nonJokers = count - jokers;

	return (nonJokers + jokers >= 3 && nonJokers + jokers <= 4);

}

// validates a series combination 
// 3+ tiles of the same colour made up of numbers in a row in an ascending order 
bool isValidSeries(const Player& player, const int selectedTiles[], int count) {
	
	Tile tiles[DECK_SIZE];
	for (int i = 0; i < count; i++) {

		tiles[i] = player.hand[selectedTiles[i]];

	}

	return isValidSeriesCore(tiles, count);

}

// validates every combination played by the player
bool isValidMultipleCombination(const Player& player, const Combination combinations[], int combinationCount) {

	for (int i = 0; i < combinationCount; i++) {

		const Combination& combination = combinations[i];

		if (!isValidGroup(player, combination.tiles, combination.count) && !isValidSeries(player, combination.tiles, combination.count)) {

			return false;

		}

	}

	return true;

}

// reads the input of the player to detect the multiple combinations he wants to play
// combinations are seperated by '|'
bool readMultipleTileSelection(const Player& player, Combination combinations[], int& combinationCount) {

	const int MAX_INPUT = 1024;
	char input[MAX_INPUT];

	while (true) {

		cout << "Enter tile indexes (use | to seperate, e.g. 1 2 3 | 4 5 6): ";
		cin.getline(input, MAX_INPUT);

		parseMultipleTileSelection(player, input, combinations, combinationCount);

		int decision = confirmSelectedCombinations(player, combinations, combinationCount);

		if (decision == 1) {

			return true;

		}
		if (decision == -1) {

			return false;

		}

		cout << "Let's try again" << endl;

	}

}

// calculates the point value of all played tile combinations
int calculateMultipleSelectedTilesPoints(const Player& player, const Combination combinations[], int combinationCount) {

	int total = 0;

	for (int c = 0; c < combinationCount; c++) {

		total += calculateSelectedTilesPoints(player, combinations[c].tiles, combinations[c].count);

	}

	return total;

}

// checks if the combinations played have a total value of 30+, required for the first turn of each player
// once this check becomes true, it stays that way
bool checkInitial30Multiple(Player& player, const Combination combinations[], int combinationCount) {

	if (player.hasInitial30) {

		return true;

	}

	int points = calculateMultipleSelectedTilesPoints(player, combinations, combinationCount);

	if (points >= 30) {

		player.hasInitial30 = true;
		return true;

	}

	cout << "You must play at least 30 points on your first move!" << endl << "You played only " << points << " points." << endl;

	return false;

}

// displayed the selected combinations and asks for verification from the player before playing them on the table
int confirmSelectedCombinations(const Player& player, const Combination combinations[], int combinationCount) {

	if (combinationCount == 0) {

		cout << "You didn't select any tiles!" << endl;
		return 0;

	}

	cout << "You selected: " << endl;

	for (int c = 0; c < combinationCount; c++) {

		cout << "Combination " << c + 1 << ": ";
		for (int t = 0; t < combinations[c].count; t++) {

			printTile(player.hand[combinations[c].tiles[t]]);
			cout << " ";

		}

		cout << endl;

	}

	char confirm;

	while (true) {

		cout << "Are you sure? (y = play, n = reselect, d = draw tile): ";
		cin >> confirm;
		cin.ignore(1024, '\n');

		if (confirm == 'y' || confirm == 'Y') {

			return 1;

		}
		else if (confirm == 'n' || confirm == 'N') {

			return 0;

		}
		else if (confirm == 'd' || confirm == 'D') {

			return -1;

		}

		cout << "Invalid data!" << endl;

	}

}

// parses the input of the player and places them into the combinations array
// prevents the selection of the same tile multiple times
void parseMultipleTileSelection(const Player& player, const char input[], Combination combinations[], int& combinationCount) {

	combinationCount = 0;

	int i = 0;

	while (input[i] != '\0') {

		combinations[combinationCount].count = 0;

		while (input[i] != '\0' && input[i] != '|') {

			if (isDigit(input[i])) {

				int number = readNumber(input, i);
				int index = number - 1;

				if (index >= 0 && index < player.handCount) {

					bool alreadySelected = false;

					for (int c = 0; c <= combinationCount; c++) {

						for (int t = 0; t < combinations[c].count; t++) {

							if (combinations[c].tiles[t] == index) {

								alreadySelected = true;
								break;

							}

						}
						
						if (alreadySelected) {

							break;

						}

					}

					if (!alreadySelected) {

						combinations[combinationCount].tiles[combinations[combinationCount].count++] = index;

					}

				}

			}
			else {

				i++;

			}

		}

		if (combinations[combinationCount].count > 0) {

			combinationCount++;

		}

		if (input[i] == '|') {

			i++;

		}

	}

}

// removes all selected tiles from the player's hand
void removeMultipleSelectedTiles(Player& player, const Combination combinations[], int combinationCount) {

	int allSelected[DECK_SIZE];
	int totalCount = 0;

	for (int c = 0; c < combinationCount; c++) {

		for (int t = 0; t < combinations[c].count; t++) {

			allSelected[totalCount++] = combinations[c].tiles[t];

		}

	}

	removeSelectedTiles(player, allSelected, totalCount);

}

// adds the selected combinations to the table
// fails if the table is full (shouldn't be possible in normal gameplay)
bool addCombinationsToTable(const Player& player, Table& table, const Combination combinations[], int combinationCount) {

	if (table.count + combinationCount > MAX_TABLE_COMBINATIONS) {

		cout << "Table is full! Cannot place more combinations." << endl;
		return false;

	}

	for (int c = 0; c < combinationCount; c++) {

		TableCombination newCombo;
		newCombo.count = 0;

		for (int t = 0; t < combinations[c].count; t++) {

			int handIndex = combinations[c].tiles[t];

			if (handIndex < 0 || handIndex >= player.handCount) {

				cout << "Invalid hand index while placing combination!" << endl;
				return false;

			}

			newCombo.tiles[newCombo.count++] = player.hand[handIndex];

		}

		table.combos[table.count++] = newCombo;

	}

	return true;

}

// let's the player choose from which table combination they want to steal
int chooseTableCombinationIndex(const Table& table) {

	if (table.count == 0) {

		return -1;

	}

	int choice = 0;

	while (choice < 1 || choice > table.count) {

		cout << "Choose table combination (1-" << table.count << "): ";
		cin >> choice;
		cin.ignore(1024, '\n');

	}

	return choice - 1;

}

// validates a group directly from a Tile array, which is used for "stolen" combos
bool isValidGroupTiles(const Tile tiles[], int count) {

	if (count < 3 || count > 4) {

		return false;

	}

	bool usedColours[4] = { false, false, false, false };
	int jokers = 0;
	int value = -1;

	for (int i = 0; i < count; i++) {

		if (tiles[i].value == JOKER_VALUE) {

			jokers++;
			continue;

		}

		if (value == -1) {

			value = tiles[i].value;

		}
		else if (tiles[i].value != value) {

			return false;

		}
		
		if (usedColours[tiles[i].colour]) {

			return false;

		}

		usedColours[tiles[i].colour] = true;

	}

	int nonJokers = count - jokers;
	return (nonJokers + jokers >= 3 && nonJokers + jokers <= 4);

}

// validates a series dirrectly from a Tile array
bool isValidSeriesTiles(const Tile tiles[], int count) {
	
	return isValidSeriesCore(tiles, count);

}

// validates if a combination is either a group of series
bool isValidCombinationTiles(const Tile tiles[], int count) {

	return isValidGroupTiles(tiles, count) || isValidSeriesTiles(tiles, count);

}

// reads a single selection of tiles from a player's hand (or allows drawing to cancel)
// used when adding tiles to the table or when selecting tiles to build a new combo during stealing
bool readSingleSelectionOrDraw(const Player& player, int selectedTiles[], int& count) {

	const int MAX_INPUT = 1024;
	char input[MAX_INPUT];

	while (true) {

		cout << "Enter tile indexes to add (e.g. 1 3 5): ";
		cin.getline(input, MAX_INPUT);

		parseSingleTileSelection(player, input, selectedTiles, count);

		int decision = confirmSelectedTilesSingle(player, selectedTiles, count);

		if (decision == 1) {

			return true;

		}
		if (decision == -1) {

			return false;

		}

		cout << "Let's try again" << endl;

	}

}

// adds one or more tiles from the player's hand to an existing table combination.
// reverts the change if the resulting table combination is invalid.
int addTilesToTableCombination(Player& player, Table& table) {

	if (table.count == 0) {

		cout << "Table is empty. Nothing to add to." << endl;
		return 0;

	}

	int tableIndex = chooseTableCombinationIndex(table);
	if (tableIndex < 0) {

		return 0;

	}

	int selected[DECK_SIZE];
	int count = 0;

	bool wantsToPlay = readSingleSelectionOrDraw(player, selected, count);
	if (!wantsToPlay) {

		return -1;

	}

	TableCombination backup = table.combos[tableIndex];

	for (int i = 0; i < count; i++) {

		if (table.combos[tableIndex].count >= DECK_SIZE) {

			cout << "Combination  is too long!" << endl;
			table.combos[tableIndex] = backup;
			return 0;

		}

		table.combos[tableIndex].tiles[table.combos[tableIndex].count++] = player.hand[selected[i]];

	}

	if (!isValidCombinationTiles(table.combos[tableIndex].tiles, table.combos[tableIndex].count)) {

		cout << "Resulting table combination is invalid. Reverting..." << endl;
		table.combos[tableIndex] = backup;
		return 0;

	}

	removeSelectedTiles(player, selected, count);
	return 1;

}

// parses a list of hand indices from a single line into selectedTiles[].
// prevents duplicates in the same selection.
void parseSingleTileSelection(const Player& player, const char input[], int selectedTiles[], int& count) {

	count = 0;

	for (int i = 0; input[i] != '\0'; i++) {

		if (isDigit(input[i])) {

			int number = readNumber(input, i);
			int index = number - 1;

			if (index >= 0 && index < player.handCount) {

				bool alreadySelected = false;
				for (int j = 0; j < count; j++) {

					if (selectedTiles[j] == index) {

						alreadySelected = true;
						break;

					}

				}

				if (!alreadySelected) {

					selectedTiles[count++] = index;

				}

			}

			i--;

		}

	}

}

// shows selected tiles and asks for a confirmation
int confirmSelectedTilesSingle(const Player& player, const int selectedTiles[], int count) {

	if (count == 0) {

		cout << "You didn't select any tiles!" << endl;
		return 0;

	}

	cout << "You selected: ";
	for (int k = 0; k < count; k++) {

		printTile(player.hand[selectedTiles[k]]);
		cout << " ";

	}
	cout << endl;

	char confirm;	
	while (true) {

		cout << "Are you sure? (y = play, n = reselect, d = draw tile): ";
		cin >> confirm;
		cin.ignore(1024, '\n');

		if (confirm == 'y' || confirm == 'Y') {

			return 1;

		}
		if (confirm == 'n' || confirm == 'N') {

			return 0;

		}
		if (confirm == 'd' || confirm == 'D') {

			return -1;

		}

		cout << "Invalid input!" << endl;
		
	}

}

// parses table positions from input into selectedPos[]
// comboCount is the length of the chosen table combination
void parseTableTilePositions(const char input[], int selectedPos[], int& count, int comboCount) {

	count = 0;

	for (int i = 0; input[i] != '\0'; i++) {

		if (isDigit(input[i])) {

			int number = readNumber(input, i);
			int pos = number - 1;

			if (pos >= 0 && pos < comboCount) {

				bool alreadySelected = false;

				for (int j = 0; j < count; j++) {

					if (selectedPos[j] == pos) {

						alreadySelected = true;
						break;

					}

				}

				if (!alreadySelected) {

					selectedPos[count++] = pos;

				}

			}

			i--;

		}

	}

}

// Prints what will be stolen from a specific table combination and asks for confirmation
int confirmStealSelection(const TableCombination combo, const int selectedPos[], int count) {

	if (count == 0) {

		cout << "You didn't select any tiles to steal!" << endl;
		return 0;

	}

	cout << "You will steal: ";
	for (int i = 0; i < count; i++) {

		printTile(combo.tiles[selectedPos[i]]);
		cout << " ";

	}
	cout << endl;

	char confirm;
	while (true) {

		cout << "Are you sure? (y = steal, n = reselect, d = draw tile): ";
		cin >> confirm;
		cin.ignore(1024, '\n');

		if (confirm == 'y' || confirm == 'Y') {

			return 1;

		}
		if (confirm == 'n' || confirm == 'N') {

			return 0;

		}
		if (confirm == 'd' || confirm == 'D') {

			return -1;

		}

		cout << "Invalid input!" << endl;

	}

}

// removes selected positions from a table combination and outputs them into stolen[]
// uses descending removal to avoid index shifting problems
void removeTilesFromTableCombination(TableCombination& combo, int selectedPos[], int count, Tile stolen[], int& stolenCount) {

	stolenCount = 0;
	sortDescending(selectedPos, count);

	for (int i = 0; i < count; i++) {

		int pos = selectedPos[i];

		stolen[stolenCount++] = combo.tiles[pos];

		for (int j = pos; j < combo.count - 1; j++) {

			combo.tiles[j] = combo.tiles[j + 1];

		}
		combo.count--;

	}

}

// adds a new combination to the table as a separate table combination
// returns false if the table is full (shouldn't be possible in normal gameplay)
bool addTileCombinationToTable(Table& table, const Tile tiles[], int count) {

	if (table.count >= MAX_TABLE_COMBINATIONS) {

		cout << "Table is full!" << endl;
		return false;

	}

	TableCombination newCombo;
	newCombo.count = count;

	for (int i = 0; i < count; i++) {

		newCombo.tiles[i] = tiles[i];

	}

	table.combos[table.count++] = newCombo;
	return true;

}

// main steal flow
// choose a source combination from which to steal
// choose which tiles to steal
// validate if the remaining tiles form a combo
// decide what to do with the stolen tiles
int stealFromTableAndCreateNewCombination(Player& player, Table& table) {

	int tableIndex = -1;
	TableCombination original;

	if (!chooseStealSource(table, tableIndex, original)) {

		return 0;

	}

	int selectedPos[DECK_SIZE];
	int posCount = 0;

	int ok = readStealPositions(original, selectedPos, posCount);
	if (ok == -1) {

		return -1;

	}
	if (ok == 0) {

		return 0;

	}

	TableCombination modified;
	Tile stolen[DECK_SIZE];
	int stolenCount = 0;

	int replaceHandIndex[DECK_SIZE];
	int replaceCount = 0;

	bool usedHand[DECK_SIZE] = { false };
	bool didSplit = false;

	TableCombination splitSecond;
	splitSecond.count = 0;

	int built = buildStealResult(player, original, selectedPos, posCount, modified, stolen, stolenCount, replaceHandIndex, replaceCount, usedHand, didSplit, splitSecond);

	if (built != 1) {

		return 0;

	}

	return runStealModeLoop(player, table, tableIndex, modified, didSplit, splitSecond, stolen, stolenCount, replaceHandIndex, replaceCount, usedHand);

}

// ensures the player has already placed their initial 30+ points before using table-modification actions
bool ensureInitial30(const Player& player) {

	if (!player.hasInitial30) {

		cout << "You must first place initial 30 points using only your hand." << endl;
		return false;

	}

	return true;

}

// prints the combinations that are about to be played from the player's hand 
void printPlayedCombinations(const Player& player, const Combination combos[], int comboCount) {

	cout << "Played: " << endl;
	for (int c = 0; c < comboCount; c++) {

		cout << "Combination " << c + 1 << ": ";
		for (int t = 0; t < combos[c].count; t++) {

			printTile(player.hand[combos[c].tiles[t]]);
			cout << " ";

		}

		cout << endl;

	}

}

// build a Tile array representing the stolen tiles + the selected tiles to validate a new combination
int buildNewComboFromStolenAndHand(const Player& player, const Tile stolen[], int stolenCount, const int handSelected[], int handCount, Tile out[]) {

	int n = 0;
	for (int i = 0; i < stolenCount; i++) {

		out[n++] = stolen[i];

	}
	for (int i = 0; i < handCount; i++) {

		out[n++] = player.hand[handSelected[i]];

	}

	return n;

}

// reads a single hand tile index to use as a joker replacement (disallows using the same tile or another joker)
int readReplacementTileIndex(const Player& player, const bool used[], const char* prompt) {

	while (true) {

		printHand(player);
		cout << prompt;
		int choice;
		cin >> choice;
		cin.ignore(1024, '\n');

		int index = choice - 1;

		if (index < 0 || index >= player.handCount) {

			cout << "Invalid index!" << endl;
			continue;

		}

		if (used[index]) {

			cout << "You already used this tile!" << endl;
			continue;

		}

		if (player.hand[index].value == JOKER_VALUE) {

			cout << "You cannot replace a joker with another joker." << endl;
			continue;

		}

		return index;

	}

}

// forces the player to place each stolen tile onto some existing table combination
// this is used when the player can't or doesn't want to use the stolen tiles to form a new combination
bool tryPlaceStolenTilesOnTable(Table& table, const Tile stolen[], int stolenCount) {

	if (stolenCount == 0) {

		return true;

	}
	if (table.count == 0) {

		return false;

	}
	
	for (int s = 0; s < stolenCount; s++) {

		Tile tile = stolen[s];

		while (true) {

			cout << "You must place stolen tile: ";
			printTile(tile);
			cout << endl;

			printTable(table);

			int targetIndex = chooseTableCombinationIndex(table);
			if (targetIndex < 0) {

				return false;

			}

			TableCombination backup = table.combos[targetIndex];

			if (table.combos[targetIndex].count >= DECK_SIZE) {

				cout << "That combination is too long. Choose another." << endl;
				continue;

			}

			table.combos[targetIndex].tiles[table.combos[targetIndex].count++] = tile;

			if (isValidCombinationTiles(table.combos[targetIndex].tiles, table.combos[targetIndex].count)) {

				cout << "Placed successfully" << endl;
				break;

			}

			table.combos[targetIndex] = backup;
			cout << "Can't place it there (combination becomes invalid). Choose another." << endl;

		}

	}

	return true;

}

// checks if the player has a tile with which to replace a stolen joker (the tile needs to be a non-joker tile)
bool hasAnyValidJokerReplacement(const Player& player, const TableCombination& modified, int jokerPos, const bool usedHand[]) {

	for (int i = 0; i < player.handCount; i++) {

		if (usedHand[i]) {

			continue;

		}

		if (player.hand[i].value == JOKER_VALUE) {

			continue;

		}

		TableCombination test = modified;
		test.tiles[jokerPos] = player.hand[i];

		if (isValidCombinationTiles(test.tiles, test.count)) {

			return true;

		}

	}

	return false;

}

// extracts series validation data
// returns false if the tiles cannot represent a potential series
bool buildSeriesData(const Tile tiles[], int count, int values[], int& valueCount, int& jokers, Colour& colour) {

	valueCount = 0;
	jokers = 0;
	bool colourSet = false;

	for (int i = 0; i < count; i++) {

		if (tiles[i].value == JOKER_VALUE) {

			jokers++;
			continue;

		}

		if (!colourSet) {

			colour = tiles[i].colour;
			colourSet = true;

		}
		else if (tiles[i].colour != colour) {

			return false;

		}

		for (int j = 0; j < valueCount; j++) {

			if (values[j] == tiles[i].value) {

				return false;

			}

		}

		values[valueCount++] = tiles[i].value;

	}

	return true;

}

// checks that tiles can form a consecutive sequence of the same colour, allowing jokers to fill gaps between sorted values.
bool isValidSeriesCore(const Tile tiles[], int count) {

	if (count < 3) {

		return false;

	}

	int values[DECK_SIZE];
	int valueCount, jokers;
	Colour colour = ORANGE;

	if (!buildSeriesData(tiles, count, values, valueCount, jokers, colour)) {

		return false;

	}

	sortAsc(values, valueCount);

	int gaps = 0;
	for (int i = 0; i < valueCount - 1; i++) {

		gaps += (values[i + 1] - values[i] - 1);

	}

	return gaps <= jokers;

}

// handles action "Add to existing table combination" for a turn.
// requires initial 30 points already completed, returns a TurnResult for the main turn loop.
TurnResult handleAddToTable(Player& player, Table& table) {

	if (!ensureInitial30(player)) {

		return TURN_CONTINUE;

	}

	int decision = addTilesToTableCombination(player, table);

	if (decision == -1) {

		return TURN_DRAW_END;

	}
	if (decision == 1) {

		return TURN_DID_SOMETHING;

	}

	cout << "Couldn't add tiles. Try again or choose another action." << endl;
	return TURN_CONTINUE;

}

// handles action "Steal tiles from table" for a turn.
// requires initial 30 points already completed, returns a TurnResult for the main turn loop.
TurnResult handleSteal(Player& player, Table& table) {

	if (!ensureInitial30(player)) {

		return TURN_CONTINUE;

	}

	int result = stealFromTableAndCreateNewCombination(player, table);

	if (result == -1) {

		return TURN_DRAW_END;

	}
	if (result == 1) {

		return TURN_DID_SOMETHING;

	}

	return TURN_CONTINUE;

}

// handles action "Play new combinations" for a turn.
// reads multiple combinations, validates them, checks initial 30 rule, adds them to the table, and removes from hand.
TurnResult handlePlayNewCombinations(Player& player, Table& table) {

	Combination combinations[DECK_SIZE];
	int combinationCount = 0;

	bool wantsToPlay = readMultipleTileSelection(player, combinations, combinationCount);
	if (!wantsToPlay) {

		return TURN_DRAW_END;

	}
	if (!isValidMultipleCombination(player, combinations, combinationCount)) {

		cout << "Invalid combination!" << endl;
		return TURN_CONTINUE;

	}
	if (!checkInitial30Multiple(player, combinations, combinationCount)) {

		cout << "Please select tiles again or draw a tile" << endl;
		return TURN_CONTINUE;

	}

	printPlayedCombinations(player, combinations, combinationCount);
	cout << endl;

	if (!addCombinationsToTable(player, table, combinations, combinationCount)) {

		cout << "Failed to place combinations on the table." << endl;
		return TURN_CONTINUE;

	}

	removeMultipleSelectedTiles(player, combinations, combinationCount);
	return TURN_DID_SOMETHING;

}

// UI loop for stealing: prints the chosen table combination and reads positions until confirmed or canceled.
// returns 1=confirmed, 0=reselect/cancel, -1=draw.
int readStealPositionsUI(const TableCombination& original, int selectedPos[], int& posCount) {

	const int MAX_INPUT = 1024;
	char input[MAX_INPUT];

	posCount = 0;

	while (true) {

		cout << "Selected table combination: ";
		printTableCombinationWithPositions(original);
		cout << endl;

		cout << "Enter positions to steal (e.g. 1 3): ";
		cin.getline(input, MAX_INPUT);

		parseTableTilePositions(input, selectedPos, posCount, original.count);

		int decision = confirmStealSelection(original, selectedPos, posCount);
		if (decision == -1) {

			return -1; //draw

		}
		if (decision == 1) {

			return 1; //confirmed

		}

		posCount = 0; //reselect

	}

}

// applies a steal selection to a chosen table combination:
// 1. handles joker stealing by forcing a valid replacement from hand
// 2. removes non-joker positions
// 3. validates the remaining combination or attempts to split it into two valid series.
int applyStealToCombination(Player& player, const TableCombination& original, const int selectedPos[], int posCount, TableCombination& modified, Tile stolen[], int& stolenCount, int replaceHandIndex[], int& replaceCount, bool usedHand[], bool& didSplit, TableCombination& splitSecond) {

	modified = original;
	stolenCount = 0;
	replaceCount = 0;
	didSplit = false;
	splitSecond.count = 0;

	int removePos[DECK_SIZE];
	int removeCount = 0;

	for (int i = 0; i < posCount; i++) {

		if (!stealSinglePos(player, original, selectedPos[i], modified, stolen, stolenCount, replaceHandIndex, replaceCount, usedHand, removePos, removeCount)) {

			return 0;

		}

	}

	removePositionsFromModified(modified, removePos, removeCount);
	return finalizeStealWithSplitIfNeeded(modified, didSplit, splitSecond);

}

// reads and validates the steal mode choice:
// c = create new combination from stolen tiles + hand tiles
// a = place stolen tiles onto existing table combinations
// s = reselect stolen tiles
// d = draw a tile (ends turn)
char chooseStealMode() {

	char mode;

	while (true) {

		cout << "Choose what to do with stolen tiles (c = create new combination, a = add stolen tiles to table, s = reselect stolen tiles, d = draw tile): ";
		cin >> mode;
		cin.ignore(1024, '\n');

		if (mode == 'c' || mode == 'C' || mode == 'a' || mode == 'A' || mode == 's' || mode == 'S' || mode == 'd' || mode == 'D') {

			return mode;

		}

		cout << "Invalid input!" << endl;

	}

}

// removes the indexes from hand, requires them to be unique (so a tile can't be removed twice)
void removeUniqueHandIndexes(Player& player, const int index[], int n) {

	bool mark[DECK_SIZE] = { false };
	int unique[DECK_SIZE];
	int u = 0;

	for (int i = 0; i < n; i++) {

		int x = index[i];
		if (x >= 0 && x < player.handCount && !mark[x]) {

			mark[x] = true;
			unique[u++] = x;

		}

	}

	removeSelectedTiles(player, unique, u);

}

// steal mode 'a': commits the modified (remaining) table combination (and optional split), then forces the player to place all stolen tiles onto the table in valid ways.
int handleModeAddToTable(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile stolen[], int stolenCount, const int replaceHandIndex[], int replaceCount) {

	Table backup = table;
	table.combos[tableIndex] = modified;

	if (didSplit) {

		if (table.count + 1 > MAX_TABLE_COMBINATIONS) {

			table = backup;
			cout << "Table is full - cannot split remaning combination." << endl;
			return 0;

		}

		table.combos[table.count++] = splitSecond;

	}

	if (!tryPlaceStolenTilesOnTable(table, stolen, stolenCount)) {

		table = backup;
		cout << "Couldn't place stolen tiles on table. Choose another option." << endl;
		return 0;

	}

	if (didSplit) {

		cout << "Remaining combination was split into two combinations." << endl;


	}

	removeUniqueHandIndexes(player, replaceHandIndex, replaceCount);

	cout << "Steal successful (tiles placed on table)." << endl;
	return 1;

}

// steal mode 'c': user selects extra hand tiles, builds a new combination with stolen tiles, 
// validates it, commits table modifications (including split), and removes all used hand tiles.
int handleModeCreateNewCombo(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile stolen[], int stolenCount, const int replaceHandIndex[], int replaceCount, const bool usedHand[]) {

	int handSelected[DECK_SIZE];
	int handCount = 0;

	if (!readSingleSelectionOrDraw(player, handSelected, handCount)) {

		return -1;

	}

	Tile newComboTiles[DECK_SIZE];
	int newCount = 0;

	if (!prepareNewComboTiles(player, stolen, stolenCount, handSelected, handCount, usedHand, newComboTiles, newCount)) {

		return 0;

	}

	int allToRemove[DECK_SIZE];
	int allCount = buildAllToRemove(replaceHandIndex, replaceCount, handSelected, handCount, allToRemove);

	if (!commitStealModeCreateNewCombo(player, table, tableIndex, modified, didSplit, splitSecond, newComboTiles, newCount, allToRemove, allCount)) {

		return 0;

	}

	return 1;

}

// attempts to split a (broken) series into two valid series (each length >= 3).
// this supports the "split" mechanic when stealing middle tiles from a long sequence.
bool trySplitSeriesIntoTwoValid(const TableCombination& in, TableCombination& outA, TableCombination& outB) {

	if (in.count < 6) {

		return false;

	}

	Tile nonJokers[DECK_SIZE];
	int nonCount;

	Tile jokers[DECK_SIZE];
	int jokerCount;

	Colour colour = ORANGE;

	int values[DECK_SIZE];
	int valueCount;

	if (!collectSeriesSplitData(in, nonJokers, nonCount, jokers, jokerCount, values, valueCount, colour)) {

		return false;

	}

	Tile sortedNon[DECK_SIZE];
	buildSortedNonJokers(nonJokers, nonCount, values, valueCount, sortedNon);
	nonCount = valueCount;

	return tryAllCutsAndJokers(sortedNon, nonCount, jokers, jokerCount, outA, outB);

}

// tries to split the remaining invalid table combination.
// currently only supports splitting series (not groups, since groups can be a maximum of 4 tiles), so it delegates to trySplitSeriesIntoTwoValid.
bool trySplitRemainingCombo(const TableCombination& in, TableCombination& outA, TableCombination& outB) {

	if (isValidCombinationTiles(in.tiles, in.count)) {

		return false;

	}

	return trySplitSeriesIntoTwoValid(in, outA, outB);

}

// checks if the remaining table combination is valid after stealing
bool isRemainingTableComboOk(const TableCombination& modified) {

	if (modified.count < 3) {

		return false;

	}

	return isValidCombinationTiles(modified.tiles, modified.count);

}

// processes stealing a single position from the original table combination.
// 1. if it's a joker, forces the player to replace it with a valid hand tile first.
// 2. if it's a normal tile, it is marked for removal from the remaining combination.
bool stealSinglePos(Player& player, const TableCombination& original, int pos, TableCombination& modified, Tile stolen[], int& stolenCount, int replaceHandIndex[], int& replaceCount, bool usedHand[], int removePos[], int& removeCount) {

	if (pos < 0 || pos >= original.count) {

		cout << "Invalid table position!" << endl;
		return false;

	}

	Tile picked = original.tiles[pos];

	if (picked.value == JOKER_VALUE) {

		if (!hasAnyValidJokerReplacement(player, modified, pos, usedHand)) {

			cout << "You cannot steal this joker. You have no valid tile to replace it with." << endl;
			return false;

		}

		while (true) {

			int handIndex = readReplacementTileIndex(player, usedHand, "Choose a tile from your hand to replace this joker (index): ");

			Tile old = modified.tiles[pos];
			modified.tiles[pos] = player.hand[handIndex];

			if (isValidCombinationTiles(modified.tiles, modified.count)) {

				usedHand[handIndex] = true;
				replaceHandIndex[replaceCount++] = handIndex;
				stolen[stolenCount++] = picked;
				return true;

			}

			modified.tiles[pos] = old;
			cout << "That tile does not fit as a replacement. Choose another." << endl;

		}

	}

	removePos[removeCount++] = pos;
	stolen[stolenCount++] = picked;
	return true;

}

// removes multiple positions from the modified combination (descending order to avoid shifting issues).
void removePositionsFromModified(TableCombination& modified, int removePos[], int removeCount) {

	sortDescending(removePos, removeCount);

	for (int i = 0; i < removeCount; i++) {

		int pos = removePos[i];

		for (int j = pos; j < modified.count - 1; j++) {

			modified.tiles[j] = modified.tiles[j + 1];

		}
		modified.count--;

	}

}

// final step after stealing:
// if remaining combo is valid -> ok,
// else try split into two series -> ok,
// else print a reason why the steal is impossible.
int finalizeStealWithSplitIfNeeded(TableCombination& modified, bool& didSplit, TableCombination& splitSecond) {

	if (isRemainingTableComboOk(modified)) {

		return 1;

	}

	TableCombination a, b;
	if (trySplitRemainingCombo(modified, a, b)) {

		modified = a;
		splitSecond = b;
		didSplit = true;
		return 1;

	}

	if (modified.count < 3) {

		cout << "You can't steal like that (remaining would be too short)." << endl;

	}
	else {

		cout << "You can't steal like that (remaining would be invalid)." << endl;

	}

	return 0;

}

// filters out any hand indexes that were already used as joker replacements during the same steal action.
int filterUnusedHandSelection(const Player& player, int handSelected[], int handCount, const bool usedHand[]) {

	int filtered[DECK_SIZE];
	int filteredCount = 0;

	for (int i = 0; i < handCount; i++) {

		int index = handSelected[i];
		if (index >= 0 && index < player.handCount && !usedHand[index]) {

			filtered[filteredCount++] = index;

		}

	}

	for (int i = 0; i < filteredCount; i++) {

		handSelected[i] = filtered[i];

	}

	return filteredCount;

}

// builds and validates the new combination for steal mode 'c' (stolen tiles + chosen hand tiles).
// returns false if all selected tiles are invalid/used or if the resulting combo is not valid.
bool prepareNewComboTiles(const Player& player, const Tile stolen[], int stolenCount, int handSelected[], int& handCount, const bool usedHand[], Tile newComboTiles[], int& newCount) {

	handCount = filterUnusedHandSelection(player, handSelected, handCount, usedHand);

	if (handCount == 0) {

		cout << "All selected tiles are already used as joker replacements. Select again." << endl;
		return false;

	}

	newCount = buildNewComboFromStolenAndHand(player, stolen, stolenCount, handSelected, handCount, newComboTiles);

	if (!isValidCombinationTiles(newComboTiles, newCount)) {

		cout << "New combination is invalid." << endl;
		return false;

	}

	return true;

}

// merges replacement-tile indexes and selected hand indexes into a single list for removal from the hand.
int buildAllToRemove(const int replaceHandIndex[], int replaceCount, const int handSelected[], int handCount, int allToRemove[]) {

	int allCount = 0;

	for (int i = 0; i < replaceCount; i++) {

		allToRemove[allCount++] = replaceHandIndex[i];

	}
	for (int i = 0; i < handCount; i++) {

		allToRemove[allCount++] = handSelected[i];

	}

	return allCount;

}

// commits steal mode 'c' changes atomically using a table backup:
// 1. writes modified remaining combo,
// 2. optionally appends splitSecond,
// 3. adds the new combination,
// 4. removes used hand tiles if everything succeeds.
bool commitStealModeCreateNewCombo(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile newComboTiles[], int newCount, const int allToRemove[], int allCount) {

	Table backup = table;

	table.combos[tableIndex] = modified;

	int needed = didSplit ? 2 : 1;
	if (table.count + needed > MAX_TABLE_COMBINATIONS) {

		table = backup;
		cout << "Table is full - cannot split remaining combination." << endl;
		return false;

	}

	if (didSplit) {

		table.combos[table.count++] = splitSecond;

	}

	if (!addTileCombinationToTable(table, newComboTiles, newCount)) {

		table = backup;
		cout << "Failed to add new combination to table (table full)." << endl;
		return false;

	}

	if (didSplit) {

		cout << "Remaining combination was split into two combinations." << endl;

	}

	removeUniqueHandIndexes(player, allToRemove, allCount);
	cout << "Steal successful!" << endl;
	return true;

}

// collects and validates data needed for splitting a series:
// 1. separates jokers and non-jokers,
// 2. enforces same colour,
// 3. enforces distinct values,
// 4. fills "values" for sorting.
bool collectSeriesSplitData(const TableCombination& in, Tile nonJokers[], int& nonCount, Tile jokers[], int& jokerCount, int values[], int& valueCount, Colour& colour) {

	nonCount = 0;
	jokerCount = 0;
	valueCount = 0;

	bool colourSet = false;

	for (int i = 0; i < in.count; i++) {

		Tile t = in.tiles[i];

		if (t.value == JOKER_VALUE) {

			jokers[jokerCount++] = t;
			continue;

		}

		if (!colourSet) {

			colour = t.colour;
			colourSet = true;

		}
		else if (t.colour != colour) {

			return false;

		}

		for (int j = 0; j < valueCount; j++) {

			if (values[j] == t.value) {

				return false;

			}

		}

		values[valueCount++] = t.value;
		nonJokers[nonCount++] = t;

	}

	return nonCount > 0;

}

// builds a sorted array of non-joker tiles based on sorted "values" (ascending)
void buildSortedNonJokers(const Tile nonJokers[], int nonCount, int values[], int valueCount, Tile sortedNon[]) {

	sortAsc(values, valueCount);

	for (int i = 0; i < valueCount; i++) {

		for (int j = 0; j < nonCount; j++) {

			if (nonJokers[j].value == values[i]) {

				sortedNon[i] = nonJokers[j];
				break;

			}

		}

	}

}

// tries all ways to split a series into left/right while distributing jokers between sides.
// returns true if it finds two valid series (each length >= 3).
bool tryAllCutsAndJokers(const Tile sortedNon[], int nonCount, const Tile jokers[], int jokerCount, TableCombination& outA, TableCombination& outB) {

	for (int cut = 1; cut < nonCount; cut++) {

		for (int jLeft = 0; jLeft <= jokerCount; jLeft++) {

			int jRight = jokerCount - jLeft;

			int leftCount = cut + jLeft;
			int rightCount = (nonCount - cut) + jRight;

			if (leftCount < 3 || rightCount < 3) {

				continue;

			}

			Tile leftTiles[DECK_SIZE];
			Tile rightTiles[DECK_SIZE];

			int li = 0;
			for (int i = 0; i < cut; i++) {

				leftTiles[li++] = sortedNon[i];

			}
			for (int i = 0; i < jLeft; i++) {

				leftTiles[li++] = jokers[i];

			}

			int ri = 0;
			for (int i = cut; i < nonCount; i++) {

				rightTiles[ri++] = sortedNon[i];

			}
			for (int i = 0; i < jRight; i++) {

				rightTiles[ri++] = jokers[jLeft + i];

			}

			if (isValidSeriesCore(leftTiles, leftCount) && isValidSeriesCore(rightTiles, rightCount)) {

				outA.count = leftCount;
				for (int i = 0; i < leftCount; i++) {

					outA.tiles[i] = leftTiles[i];

				}
				
				outB.count = rightCount;
				for (int i = 0; i < rightCount; i++) {

					outB.tiles[i] = rightTiles[i];

				}

				return true;

			}

		}

	}

	return false;

}

// helper for steal flow: verifies the table isn't empty and lets the player choose a source combination.
bool chooseStealSource(const Table& table, int& tableIndex, TableCombination& original) {

	if (table.count == 0) {

		cout << "Table is empty." << endl;
		return false;

	}

	tableIndex = chooseTableCombinationIndex(table);
	if (tableIndex < 0) {

		return false;

	}

	original = table.combos[tableIndex];
	return true;

}

// wrapper around the steal positions UI to keep naming consistent in the steal flow.
int readStealPositions(const TableCombination& original, int selectedPos[], int& posCount) {

	posCount = 0;
	return readStealPositionsUI(original, selectedPos, posCount);

}

// builds the result of the steal attempt (remaining combo, stolen tiles, replacements, split state).
// prints an error message if the steal is not allowed.
int buildStealResult(Player& player, const TableCombination& original, const int selectedPos[], int posCount, TableCombination& modified, Tile stolen[], int& stolenCount, int replaceHandIndex[], int& replaceCount, bool usedHand[], bool& didSplit, TableCombination& splitSecond) {

	if (!applyStealToCombination(player, original, selectedPos, posCount, modified, stolen, stolenCount, replaceHandIndex, replaceCount, usedHand, didSplit, splitSecond)) {

		cout << "Steal failed. Reselect tiles or choose another action." << endl;
		return 0;

	}

	return 1;

}

// runs the post-steal mode loop until the player completes a valid action, reselects, or draws.
// delegates to handleModeAddToTable / handleModeCreateNewCombo for the actual commit logic.
int runStealModeLoop(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile stolen[], int stolenCount, const int replaceHandIndex[], int replaceCount, const bool usedHand[]) {

	while (true) {

		char mode = chooseStealMode();

		if (mode == 'd' || mode == 'D') {

			return -1;

		}
		if (mode == 's' || mode == 'S') {

			return 0;

		}
		if (mode == 'a' || mode == 'A') {

			int r = handleModeAddToTable(player, table, tableIndex, modified, didSplit, splitSecond, stolen, stolenCount, replaceHandIndex, replaceCount);
			if (r != 0) {

				return r;

			}

			continue;

		}

		int r = handleModeCreateNewCombo(player, table, tableIndex, modified, didSplit, splitSecond, stolen, stolenCount, replaceHandIndex, replaceCount, usedHand);

		if (r != 0) {

			return r;

		}

	}

}