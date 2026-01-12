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

        cout << "Game cannot start." << endl;
        return 1;

    }

    int playerIndex = 0;

    while (true) {

        playTurn(players[playerIndex], playerIndex);

        //TODO: add check for when the game ends

        playerIndex++;

        if (playerIndex == numPlayers) {

            playerIndex = 0;

        }

    }

    return 0;

}