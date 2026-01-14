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

int confirmSelectedTiles(const Player& player, int selectedTiles[], int count) {

	if (count == 0) {

		cout << "You didn't select any tiles!" << endl;
		return false;

	}

	char confirm;

	cout << "You selected: ";
	for (int i = 0; i < count; i++) {

		printTile(player.hand[selectedTiles[i]]);
		cout << " ";

	}
	cout << endl;

	while (true) {

		cout << "Are you sure? (y = play, n = reselect, d = draw tile): ";
		cin >> confirm;
		cin.ignore(1024, '\n');

		if (confirm == 'Y' || confirm == 'y') {

			return 1;

		}
		if (confirm == 'N' || confirm == 'n') {

			cout << "Let's try again" << endl;
			return 0;

		}
		if (confirm == 'D' || confirm == 'd') {

			return -1;

		}

		cout << "Invalid input!" << endl;

	}

}

bool readTileSelection(const Player& player, int selectedTiles[], int& count) { 

	const int MAX_INPUT = 1024;
	char input[MAX_INPUT];

	while (true) {

		count = 0;
		printHand(player);

		cout << "Enter the indexes of the tiles you want to play (e.g. 1 3 5): ";
		cin.getline(input, MAX_INPUT);

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

						selectedTiles[count] = index;
						count++;

					}

				}

			}


		}

		int decision = confirmSelectedTiles(player, selectedTiles, count);

		if (decision == 1) {

			return true;

		}
		if (decision == -1) {

			return false;

		}

	}

}

int chooseTurnAction() {

	int choice = 0;

	while (choice != 1 && choice != 2) {

		cout << "Choose Action" << endl << "1 - Play tiles" << endl << "2 - Draw a tile" << endl << "Your choice: ";
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

void playTurn(Player& player, int playerIndex) {

	cout << endl << "--- Player " << playerIndex + 1 << "'s turn ---" << endl;

	while (true) {

		printHand(player);

		int action = chooseTurnAction();

		if (action == 2) {

			drawTileForPlayer(player);
			return;

		}

		int selectedTiles[DECK_SIZE];
		int count = 0;

		bool wantsToPlay = readTileSelection(player, selectedTiles, count);

		if (!wantsToPlay) {

			drawTileForPlayer(player);
			return;

		}

		if (!isValidCombination(player, selectedTiles, count)) {

			cout << "Invalid combination!" << endl;
			continue;

		}

		if (!checkInitial30(player, selectedTiles, count)) {

			cout << "Please select tiles again or draw a tile." << endl;
			continue;

		}

		cout << "Player " << playerIndex + 1 << " played: ";
		for (int i = 0; i < count; i++) {

			printTile(player.hand[selectedTiles[i]]);
			cout << " ";

		}

		cout << endl;

		removeSelectedTiles(player, selectedTiles, count);
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

bool checkInitial30(Player& player, const int selectedTiles[], int count) {

	if (player.hasInitial30) {

		return true;

	}

	int points = calculateSelectedTilesPoints(player, selectedTiles, count);

	if (points >= 30) {

		player.hasInitial30 = true;
		return true;

	}

	cout << "You must play at least 30 points on your first move!" << endl << "You played only " << points << " points." << endl;

	return false;

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

	for (int i = 1; i < valueCount - 1; i++) {

		gaps += (values[i + 1] - values[i] - 1);

	}

	return gaps <= jokers;

}

bool isValidCombination(const Player& player, const int selectedTiles[], int count) {

	return isValidGroup(player, selectedTiles, count) || isValidSeries(player, selectedTiles, count);

}