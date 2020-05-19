#ifndef TABLE
#define TABLE

#include <iostream>
#include "card.hpp"
#include "deck.hpp"
using namespace std;

// A class containing the draw and discard piles and acting as an interface for interacting with them.
class Table
{
    public:
        Table();
        void initialize();
        int getTotalCards() const;
        bool canDrawCard() const;
        bool canDrawCards( int ) const;
        Card drawCard();
        void playCard( Card, int wildColor );
        Card getStock() const;
    private:
        Deck draw;
        Deck discard;
};

#endif