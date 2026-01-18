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
* <containts player related functions and structures>
*
*/
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