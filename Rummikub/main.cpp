#include <iostream>
#include <ctime>
#include "deck.h"
#include "player.h"
#include "ui.h"
#include "game.h"

using namespace std;

int main()
{
    
    initDeck();
    shuffleDeck();

    int numPlayers = chooseNumberOfPlayers();

    Player players[MAX_PLAYERS];

    if (!initializePlayers(players, numPlayers)) {

        cout << "Game cannot start: not enough tiles for initial hands." << endl;
        return 1;

    }

    int selectedTiles[INITIAL_HAND_SIZE]; 
    int count = 0;

    cout << endl << "Player 1, select your tiles:" << endl;
    readTileSelection(players[0], selectedTiles, count);

    cout << "Tiles chosen by player 1: ";
    for (int i = 0; i < count; i++) {

        printTile(players[0].hand[selectedTiles[i]]);
        cout << " ";

    }
    cout << endl;

    return 0;

}