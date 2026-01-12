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

bool confirmSelectedTiles(const Player& player, int selectedTiles[], int count) {

	char confirm;

	cout << "You selected: ";
	for (int i = 0; i < count; i++) {

		printTile(player.hand[selectedTiles[i]]);
		cout << " ";

	}
	cout << endl;

	while (true) {

		cout << "Are you sure? (y/n): ";
		cin >> confirm;
		cin.ignore(1024, '\n');

		if (confirm == 'Y' || confirm == 'y') {

			return true;

		}
		else if (confirm == 'N' || confirm == 'n') {

			cout << "Let's try again" << endl;
			return false;

		}
		else {

			cout << "Invalid input!" << endl;

		}

	}

}

void readTileSelection(const Player& player, int selectedTiles[], int& count) { 

	const int MAX_INPUT = 1024;
	char input[MAX_INPUT];
	bool hasConfirmed = false;

	while (!hasConfirmed) {

		count = 0;

		printHand(player);

		cout << "Enter the indexes of the tiles you want to play (e.g. 1 3 5 or 123): ";
		cin.getline(input, MAX_INPUT);

		for (int i = 0; input[i] != '\0'; i++) {

			if (isDigit(input[i])) {

				int index = (input[i] - '0') - 1;

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

		hasConfirmed = confirmSelectedTiles(player, selectedTiles, count);

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
	printHand(player);

	int action = chooseTurnAction();

	if (action == 2) {

		drawTileForPlayer(player);
		return;

	}

	int selectedTiles[DECK_SIZE];
	int count = 0;

	readTileSelection(player, selectedTiles, count);

	cout << "Player " << playerIndex + 1 << " played: ";
	for (int i = 0; i < count; i++) {

		printTile(player.hand[selectedTiles[i]]);
		cout << " ";

	}

	cout << endl;

	removeSelectedTiles(player, selectedTiles, count);

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