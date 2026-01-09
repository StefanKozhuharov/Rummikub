#include "player.h"
#include <iostream>
using namespace std;

bool addToHand(Player& player, Tile tile) {

	if (player.handCount < DECK_SIZE) {

		player.hand[player.handCount] = tile;
		player.handCount++;

	}
	else {

		return false;

	}

	return true;

}

bool drawPlayerHand(Player& player) {

	for (int i = 0; i < INITIAL_HAND_SIZE; i++) {

		Tile tile;
		if (!drawTile(tile)) {

			return false;

		}
		
		if (!addToHand(player, tile)) {

			return false;

		}

	}

	return true;

}