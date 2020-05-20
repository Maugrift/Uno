#ifndef GAME
#define GAME

#include <iostream>
#include "player.hpp"
#include "table.hpp"
using namespace std;

const int MAX_PLAYERS = 6;
const int STARTING_HAND_SIZE = 7;

// A class to contain all game objects and facilitate interactions between them.
class Game
{
    public:
        Game( string[], int );
        void initializeRound();
        void nextPlayer();
        void printTurnHeader() const;
        void processPlayerTurn();
        bool roundIsOver() const;
        Player& getRoundWinner();
        void scoreRound();
        void printScores() const;
        bool gameIsOver() const;
    private:
        Table table;
        Player players[ MAX_PLAYERS ];
        int nPlayers;
        int currentPlayerIndex;
        bool reverse;
        bool skip;
        int wildColor;

        int getColorInput() const;
        int getNextPlayerIndex() const;
        void drawCard();
        Card getCardInput() const;
        void drawUpTo( Player&, int );
        void processCardAction( Card );
};

#endif
