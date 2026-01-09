#include <iostream>
#include "ui.h"

using namespace std;

void setConsoleColour(Colour colour) {

	switch (colour) {

	case RED:
		cout << "\033[31m";
		break;
	case BLACK:
		cout << "\033[97;40m"; //will use white text colour, since background is black
		break;
	case BLUE:
		cout << "\033[34m";
		break;
	case ORANGE:
		cout << "\033[33m";
		break;

	}

}

void resetConsoleColour() {

	cout << "\033[0m";

}

void printTile(const Tile& tile) {

	setConsoleColour(tile.colour);

	if (tile.value == JOKER_VALUE) {

		cout << "J";

	}
	else {

		cout << tile.value;

	}

	resetConsoleColour();

}

void printHand(const Player& player) {

	cout << "Your hand (" << player.handCount << " tiles left):" << endl;

	for (int i = 0; i < player.handCount; i++) {

		cout << "[" << i + 1 << "] ";
		printTile(player.hand[i]);
		cout << " ";

	}

	cout << endl;

}