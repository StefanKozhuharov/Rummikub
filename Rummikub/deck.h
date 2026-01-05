#pragma once
const int DECK_SIZE = 106;
const int MIN_TILE_VALUE = 1;
const int MAX_TILE_VALUE = 13;
const int JOKER_VALUE = 0;

enum Color {

    ORANGE,
    RED,
    BLUE,
    BLACK,

};

struct Tile {

    int value;
    Color color;

};

extern Tile deck[DECK_SIZE];
extern int remainingTiles;

void initDeck();
void shuffleDeck();
bool drawTile(Tile& drawnTile);