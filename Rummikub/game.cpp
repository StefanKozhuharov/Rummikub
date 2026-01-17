#include "game.h"
#include "ui.h"
#include <iostream>

using namespace std;

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

void printTableCombinationWithPositions(const TableCombination& combo) {

	for (int i = 0; i < combo.count; i++) {

		cout << "[" << i + 1 << "]";
		printTile(combo.tiles[i]);
		cout << " ";

	}

	cout << endl;

}

int chooseNumberOfPlayers() {

	int numPlayers = 0;

	while (numPlayers < 2 || numPlayers > 4) {

		cout << "Enter number of players (2-4): ";
		cin >> numPlayers;
		cin.ignore(1024, '\n');

	}

	return numPlayers;

}

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

bool isValidSeries(const Player& player, const int selectedTiles[], int count) {
	
	Tile tiles[DECK_SIZE];
	for (int i = 0; i < count; i++) {

		tiles[i] = player.hand[selectedTiles[i]];

	}

	return isValidSeriesCore(tiles, count);

}

bool isValidMultipleCombination(const Player& player, const Combination combinations[], int combinationCount) {

	for (int i = 0; i < combinationCount; i++) {

		const Combination& combination = combinations[i];

		if (!isValidGroup(player, combination.tiles, combination.count) && !isValidSeries(player, combination.tiles, combination.count)) {

			return false;

		}

	}

	return true;

}

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

int calculateMultipleSelectedTilesPoints(const Player& player, const Combination combinations[], int combinationCount) {

	int total = 0;

	for (int c = 0; c < combinationCount; c++) {

		total += calculateSelectedTilesPoints(player, combinations[c].tiles, combinations[c].count);

	}

	return total;

}

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

bool isValidSeriesTiles(const Tile tiles[], int count) {
	
	return isValidSeriesCore(tiles, count);

}

bool isValidCombinationTiles(const Tile tiles[], int count) {

	return isValidGroupTiles(tiles, count) || isValidSeriesTiles(tiles, count);

}

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

int stealFromTableAndCreateNewCombination(Player& player, Table& table) {
	
	if (table.count == 0) {

		cout << "Table is empty." << endl;
		return 0;

	}

	int tableIndex = chooseTableCombinationIndex(table);
	if (tableIndex < 0) {

		return 0;

	}

	TableCombination original = table.combos[tableIndex];

	int selectedPos[DECK_SIZE];
	int posCount = 0;
	int ok = readStealPositionsUI(original, selectedPos, posCount);

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

	if (!applyStealToCombination(player, original, selectedPos, posCount, modified, stolen, stolenCount, replaceHandIndex, replaceCount, usedHand, didSplit, splitSecond)) {

		return 0;

	}

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

bool ensureInitial30(const Player& player) {

	if (!player.hasInitial30) {

		cout << "You must first place initial 30 points using only your hand." << endl;
		return false;

	}

	return true;

}

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

bool validateRemainingTableCombo(const TableCombination& modified) {

	if (modified.count < 3) {

		cout << "You can't steal like that (remaining would be too short)." << endl;
		return false;

	}
	if (!isValidCombinationTiles(modified.tiles, modified.count)) {

		cout << "You can't steal like that (remaining would be invalid)." << endl;
		return false;

	}

	return true;

}

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

	cout << "Steal failed. Try again." << endl;
	return TURN_CONTINUE;

}

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

int applyStealToCombination(Player& player, const TableCombination& original, const int selectedPos[], int posCount, TableCombination& modified, Tile stolen[], int& stolenCount, int replaceHandIndex[], int& replaceCount, bool usedHand[], bool& didSplit, TableCombination& splitSecond) {

	modified = original;

	stolenCount = 0;
	replaceCount = 0;

	didSplit = false;
	splitSecond.count = 0;

	int removePos[DECK_SIZE];
	int removeCount = 0;

	for (int i = 0; i < posCount; i++) {

		int pos = selectedPos[i];
		if (pos < 0 || pos >= original.count) {

			cout << "Invalid table position!" << endl;
			return 0;

		}

		Tile picked = original.tiles[pos];

		if (picked.value == JOKER_VALUE) {

			if (!hasAnyValidJokerReplacement(player, modified, pos, usedHand)) {

				cout << "You cannot steal this joker. You have no valid tile to replace it with." << endl;
				return 0;

			}

			while (true) {

				int handIndex = readReplacementTileIndex(player, usedHand, "Choose a tile from your hand to replace this joker (index): ");

				Tile old = modified.tiles[pos];
				modified.tiles[pos] = player.hand[handIndex];

				if (isValidCombinationTiles(modified.tiles, modified.count)) {

					usedHand[handIndex] = true;
					replaceHandIndex[replaceCount++] = handIndex;
					stolen[stolenCount++] = picked;
					break;

				}

				modified.tiles[pos] = old;
				cout << "That tile does not fit as a replacement. Choose another." << endl;

			}

		}
		else {

			removePos[removeCount++] = pos;
			stolen[stolenCount++] = picked;

		}

	}

	sortDescending(removePos, removeCount);

	for (int i = 0; i < removeCount; i++) {

		int pos = removePos[i];
		for (int j = pos; j < modified.count - 1; j++) {

			modified.tiles[j] = modified.tiles[j + 1];

		}
		modified.count--;

	}

	if (validateRemainingTableCombo(modified)) {

		return 1;

	}
	
	TableCombination a, b;
	if (trySplitRemainingCombo(modified, a, b)) {

		modified = a;
		splitSecond = b;
		didSplit = true;

		return 1;

	}

	return 0;

}

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

int handleModeCreateNewCombo(Player& player, Table& table, int tableIndex, const TableCombination& modified, bool didSplit, const TableCombination& splitSecond, const Tile stolen[], int stolenCount, const int replaceHandIndex[], int replaceCount, const bool usedHand[]) {

	int handSelected[DECK_SIZE];
	int handCount = 0;

	bool wantsToPlay = readSingleSelectionOrDraw(player, handSelected, handCount);
	if (!wantsToPlay) {

		return -1;

	}

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

	handCount = filteredCount;

	if (handCount == 0) {

		cout << "All selected tiles are already used as joker replacements. Select again." << endl;
		return 0;

	}

	Tile newComboTiles[DECK_SIZE];
	int newCount = buildNewComboFromStolenAndHand(player, stolen, stolenCount, handSelected, handCount, newComboTiles);

	if (!isValidCombinationTiles(newComboTiles, newCount)) {

		cout << "New combination is invalid." << endl;
		return 0;

	}

	int allToRemove[DECK_SIZE];
	int allCount = 0;

	for (int i = 0; i < replaceCount; i++) {

		allToRemove[allCount++] = replaceHandIndex[i];

	}

	for (int i = 0; i < handCount; i++) {

		allToRemove[allCount++] = handSelected[i];

	}

	Table backup = table;

	table.combos[tableIndex] = modified;

	int needed = didSplit ? 2 : 1;
		
	if (table.count + needed > MAX_TABLE_COMBINATIONS) {

		table = backup;
		cout << "Table is full - cannot split remaining combination." << endl;
		return 0;

	}

	if (didSplit) {

		table.combos[table.count++] = splitSecond;

	}

	if (!addTileCombinationToTable(table, newComboTiles, newCount)) {

		table = backup;
		cout << "Failed to add new combination to table (table full)." << endl;
		return 0;

	}

	if (didSplit) {

		cout << "Remaining combination was split into two combinations." << endl;

	}
	removeUniqueHandIndexes(player, allToRemove, allCount);

	cout << "Steal successful!" << endl;
	return 1;

}

bool trySplitSeriesIntoTwoValid(const TableCombination& in, TableCombination& outA, TableCombination& outB) {

	if (in.count < 6) {

		return false;

	}

	Tile nonJokers[DECK_SIZE];
	int nonCount = 0;

	Tile jokers[DECK_SIZE];
	int jokerCount = 0;

	Colour colour = ORANGE;
	bool colourSet = false;

	int values[DECK_SIZE];
	int valueCount = 0;

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

	if (nonCount == 0) {

		return false;

	}

	sortAsc(values, valueCount);

	Tile sortedNon[DECK_SIZE];
	for (int i = 0; i < valueCount; i++) {

		for (int j = 0; j < nonCount; j++) {

			if (nonJokers[j].value == values[i]) {

				sortedNon[i] = nonJokers[j];
				break;

			}

		}

	}

	nonCount = valueCount;

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

bool trySplitRemainingCombo(const TableCombination& in, TableCombination& outA, TableCombination& outB) {

	if (isValidCombinationTiles(in.tiles, in.count)) {

		return false;

	}

	return trySplitSeriesIntoTwoValid(in, outA, outB);

}