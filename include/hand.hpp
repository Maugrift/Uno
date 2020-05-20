#ifndef HAND
#define HAND

#include "card.hpp"
using namespace std;

// A list of cards that is always sorted
class Hand
{
    public:
        Hand();
        void print() const;
        void printContents() const;
        Card getCardAt( int ) const;
        void add( Card );
        int getSize() const;
        bool isFull() const;
        bool isEmpty() const;
        void removeCardAt( int );
        void clear();
        int find( Card ) const;
        int findString( string s ) const;
        int getScore() const;
    private:
        int size; // The current size of the hand
        int capacity; // The maximum capacity of the hand
        Card cards[ TOTAL_CARDS - 1 ];
        bool isSorted() const;
};

#endif