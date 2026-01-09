#pragma once
#include "deck.h"
#include "player.h"

void setConsoleColour(Colour colour);
void resetConsoleColour();

void printTile(const Tile& tile);
void printHand(const Player& player);