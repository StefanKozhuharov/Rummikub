#include "deck.h"
#include <cstdlib>

Tile deck[DECK_SIZE];
int remainingTiles;

void initDeck() {

    remainingTiles = 0;

    for (int i = 0; i < 2; i++) {

        for (int color = ORANGE; color <= BLACK; color++) {

            for (int value = 1; value <= MAX_TILE_VALUE; value++) {

                deck[remainingTiles].value = value;
                deck[remainingTiles].color = (Color)color;
                remainingTiles++;

            }

        }

    }

    deck[remainingTiles++] = { JOKER_VALUE, BLACK };
    deck[remainingTiles++] = { JOKER_VALUE, RED };

}

void shuffleDeck() {

    for (int i = remainingTiles - 1; i > 0; i--) {

        int j = rand() % (i + 1);

        Tile temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;

    }

}

bool drawTile(Tile& drawnTile) {

    if (remainingTiles == 0) {

        return false;

    }

    drawnTile = deck[remainingTiles - 1];
    remainingTiles--;

    return true;

}