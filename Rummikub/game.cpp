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

bool confirmSelectedTiles(Player player, int selectedTiles[], int count) {

	char confirm;

	cout << "You selected: ";
	for (int i = 0; i < count; i++) {

		printTile(player.hand[selectedTiles[i]]);
		cout << " ";

	}
	cout << endl;

	cout << "Are you sure? (y/n): ";
	cin >> confirm;
	cin.ignore(1024, '\n');

	while (true) {

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