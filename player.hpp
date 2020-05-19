#ifndef PLAYER
#define PLAYER

#include "hand.hpp"
#include "table.hpp"
using namespace std;

class Player
{
    public:
        Player();
        Player( string );
        string getName() const;
        int getScore() const;
        Hand& getHand();
        void setScore( int );
        Card drawCard( Table& );
        void drawCards( int nCards, Table& );
        void playCard( Card, Table&, int wildColor );
        void playCardIndex( int, Table&, int wildColor );
    private:
        string name;
        int score;
        Hand hand;
};

#endif