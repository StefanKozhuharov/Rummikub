#include "game.h"
#include "ui.h"
#include <iostream>

using namespace std;

bool isDigit(char symbol) {

	return symbol >= '0' && symbol <= '9';

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

int chooseTurnAction() {

	int choice = 0;

	while (choice < 1 || choice > 3) {

		cout << "Choose Action" << endl << "1 - Play tiles" << endl << "2 - Draw a tile" << endl << "3 - Add tile(s) to existing table combination" << endl << "Your choice: ";
		cin >> choice;
		cin.ignore(1024, '\n');

	}

	return choice;

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

	while (true) {

		printTable(table);
		printHand(player);

		int action = chooseTurnAction();

		if (action == 2) {

			drawTileForPlayer(player);
			return;

		}

		if (action == 3 && !player.hasInitial30) {

			cout << "You must first place initial 30 points using only your hand." << endl;
			continue;

		}

		if (action == 3) {

			int decision = addTilesToTableCombination(player, table);
			if (decision == -1) {

				drawTileForPlayer(player);
				return;

			}

			if (decision == 1) {

				return;

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

		cout << "Player " << playerIndex + 1 << " played:" << endl;
		for (int c = 0; c < combinationCount; c++) {

			cout << "Combination " << c + 1 << ": ";
			for (int t = 0; t < combinations[c].count; t++) {

				printTile(player.hand[combinations[c].tiles[t]]);
				cout << " ";

			}

			cout << endl;

		}

		cout << endl;

		if (!addCombinationsToTable(player, table, combinations, combinationCount)) {

			cout << "Failed to place combinations on the table." << endl;
			continue;

		}

		removeMultipleSelectedTiles(player, combinations, combinationCount);
		return;

	}

}

void removeSelectedTiles(Player& player, int selectedTiles[], int count) {

	for (int i = 0; i < count - 1; i++) {

		for (int j = i + 1; j < count; j++) {

			if (selectedTiles[i] < selectedTiles[j]) {

				int temp = selectedTiles[i];
				selectedTiles[i] = selectedTiles[j];
				selectedTiles[j] = temp;

			}

		}

	}

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

				int number = 0;

				while (isDigit(input[i])) {

					number = number * 10 + (input[i] - '0');
					i++;

				}

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

			int number = 0;
			while (isDigit(input[i])) {

				number = number * 10 + (input[i] - '0');
				i++;

			}

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