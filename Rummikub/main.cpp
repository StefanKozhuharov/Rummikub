#include <iostream>
#include <ctime>
#include "player.h"
#include "ui.h"
using namespace std;

int main()
{
    
    srand(time(NULL));

    initDeck();
    shuffleDeck();

    Player player;
    drawPlayerHand(player);

    printHand(player);

    return 0;

}