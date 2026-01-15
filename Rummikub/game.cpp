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

		if (action == 3) {

			drawTileForPlayer(player);
			return;

		}

		if (action == 4) {

			if (!ensureInitial30(player)) {

				continue;

			}

			int result = stealFromTableAndCreateNewCombination(player, table);
			if (result == -1) {

				drawTileForPlayer(player);
				return;

			}
			if (result == 1) {

				didSomething = true;
				continue;

			}

			cout << "Steal failed. Try again." << endl;
			continue;

		}

		if (action == 5) {

			return;

		}

		if (action == 2) {

			if (!ensureInitial30(player)) {

				continue;

			}

			int decision = addTilesToTableCombination(player, table);
			if (decision == -1) {

				drawTileForPlayer(player);
				return;

			}

			if (decision == 1) {

				didSomething = true;
				continue;

			}

			cout << "Couldn't add tiles. Try again or choose another action." << endl;
			continue;

		}

		Combination combinations[DECK_SIZE];
		int combinationCount = 0;

		bool wantsToPlay = readMultipleTileSelection(player, combinations, combinationCount);

		if (!wantsToPlay) {

			drawTileForPlayer(player);
			return;

		}

		if (!isValidMultipleCombination(player, combinations, combinationCount)) {

			cout << "Invalid combination!" << endl;
			continue;

		}

		if (!checkInitial30Multiple(player, combinations, combinationCount)) {

			cout << "Please select tiles again or draw a tile." << endl;
			continue;

		}

		printPlayedCombinations(player, combinations, combinationCount);

		cout << endl;

		if (!addCombinationsToTable(player, table, combinations, combinationCount)) {

			cout << "Failed to place combinations on the table." << endl;
			continue;

		}

		removeMultipleSelectedTiles(player, combinations, combinationCount);

		didSomething = true;
		continue;

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

	if (count < 3) {

		return false;

	}

	int values[DECK_SIZE];
	int valueCount = 0;
	int jokers = 0;
	Colour colour = ORANGE;
	bool colourSet = false;

	for (int i = 0; i < count; i++) {

		Tile tile = player.hand[selectedTiles[i]];

		if (tile.value == JOKER_VALUE) {

			jokers++;
			continue;

		}

		if (!colourSet) {

			colour = tile.colour;
			colourSet = true;

		}
		else if (tile.colour != colour) {

			return false;

		}

		for (int j = 0; j < valueCount; j++) {

			if (values[j] == tile.value) {

				return false;

			}

		}

		values[valueCount] = tile.value;
		valueCount++;

	}

	for (int i = 0; i < valueCount - 1; i++) {

		for (int j = i + 1; j < valueCount; j++) {

			if (values[i] > values[j]) {

				int temp = values[i];
				values[i] = values[j];
				values[j] = temp;

			}

		}

	}

	int gaps = 0;

	for (int i = 0; i < valueCount - 1; i++) {

		gaps += (values[i + 1] - values[i] - 1);

	}

	return gaps <= jokers;

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

	if (count < 3) {

		return false;

	}

	int values[DECK_SIZE];
	int valueCount = 0;
	int jokers = 0;
	Colour colour = ORANGE;
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

	for (int i = 0; i < valueCount - 1; i++) {

		for (int j = i + 1; j < valueCount; j++) {

			if (values[i] > values[j]) {

				int temp = values[i];
				values[i] = values[j];
				values[j] = temp;

			}

		}

	}

	int gaps = 0;
	for (int i = 0; i < valueCount - 1; i++) {

		gaps += (values[i + 1] - values[i] - 1);

	}

	return gaps <= jokers;

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

	const int MAX_INPUT = 1024;
	char input[MAX_INPUT];

	int selectedPos[DECK_SIZE];
	int posCount = 0;

	while (true) {

		cout << "Selected table combination: ";
		printTableCombinationWithPositions(original);
		cout << endl;

		cout << "Enter positions to steal (e.g. 1 3): ";
		cin.getline(input, MAX_INPUT);

		parseTableTilePositions(input, selectedPos, posCount, original.count);

		int decision = confirmStealSelection(original, selectedPos, posCount);
		if (decision == -1) {

			return -1;

		}
		if (decision == 1) {

			break;

		}

		posCount = 0;

	}

	TableCombination modified = original;

	Tile stolen[DECK_SIZE];
	int stolenCount = 0;

	int selectedCopy[DECK_SIZE];
	for (int i = 0; i < posCount; i++) {

		selectedCopy[i] = selectedPos[i];

	}

	removeTilesFromTableCombination(modified, selectedCopy, posCount, stolen, stolenCount);

	if (!validateRemainingTableCombo(modified)) {

		return 0;

	}

	while (true) {

		int handSelected[DECK_SIZE];
		int handCount = 0;

		bool wantsToPlay = readSingleSelectionOrDraw(player, handSelected, handCount);
		if (!wantsToPlay) {

			return -1;

		}

		Tile newComboTiles[DECK_SIZE];
		int newCount = buildNewComboFromStolenAndHand(player, stolen, stolenCount, handSelected, handCount, newComboTiles);

		if (isValidCombinationTiles(newComboTiles, newCount)) {

			table.combos[tableIndex] = modified;
			removeSelectedTiles(player, handSelected, handCount);

			if (!addTileCombinationToTable(table, newComboTiles, newCount)) {

				cout << "Failed to add new combination to table (table full)." << endl;
				//TODO rollback (isn't neccesary the table shouldn't be full realisticly)
				return 0;

			}

			cout << "Steal successful!" << endl;
			return 1;

		}

		cout << "New combination is invalid." << endl;

		char option;
		while (true) {

			cout << "Do you want to try again? (r = reselect hand tiles, s = reselect stolen tiles, d = draw tile): ";
			cin >> option;
			cin.ignore(1024, '\n');

			if (option == 'r' || option == 'R') {

				break;

			}
			if (option == 's' || option == 'S') {

				return 0;

			}
			if (option == 'd' || option == 'D') {

				return -1;

			}

			cout << "Invalid input!" << endl;

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