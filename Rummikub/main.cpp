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
* <main file for running the project>
*
*/

#include <iostream>
#include <ctime>
#include "deck.h"
#include "player.h"
#include "ui.h"
#include "game.h"
#include "table.h"

using namespace std;

int main()
{
    
    srand((unsigned)time(nullptr));

    initDeck();
    shuffleDeck();

    int numPlayers = chooseNumberOfPlayers();
    Player players[MAX_PLAYERS];

    if (!initializePlayers(players, numPlayers)) {

        cout << "Game cannot start." << endl;
        return 1;

    }

    Table table;
    table.count = 0;

    int currentPlayer = 0;
    int winnerIndex = -1;

    while (!isGameOver(players, numPlayers, winnerIndex)) {

        playTurn(players[currentPlayer], currentPlayer, table);

        currentPlayer++;

        if (currentPlayer == numPlayers) {

            currentPlayer = 0;

        }

    }

    cout << endl << "=== GAME OVER ===" << endl;
    cout << "Winner is Player " << winnerIndex + 1 << endl;
    
    cout << "Final scores:" << endl;
    for (int i = 0; i < numPlayers; i++) {

        cout << "Player " << i + 1 << ": " << calculateHandPoints(players[i]) << " points" << endl;

    }

    return 0;

}