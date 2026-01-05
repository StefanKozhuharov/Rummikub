#include "player.h"
#include <iostream>
using namespace std;

void addToHand(Player& player, Tile tile) {

	if (player.handCount < DECK_SIZE) {

		player.hand[player.handCount] = tile;
		player.handCount++;

	}
	else {

		cout << "Your hand is full!" << endl;

	}

}

void drawPlayerHand(Player& player) {

	player.handCount = 0;
	player.hasInitial30 = false;

	for (int i = 0; i < INITIAL_HAND_SIZE; i++) {

		Tile tile;
		if (drawTile(tile)) {

			addToHand(player, tile);

		}
		else {

			cout << "There are no tiles left!" << endl;
			break;

		}

	}

}