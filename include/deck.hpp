#ifndef DECK
#define DECK

#include "card.hpp"
using namespace std;

// A stack-like implementation of a deck of cards
class Deck
{
    public:
        Deck();
        void print() const;
        void initialize();
        void push( Card );
        Card pop();
        Card peek() const;
        int getSize() const;
        bool isFull() const;
        bool isEmpty() const;
        void clear();
        void shuffle();
    private:
        int size; // The current size of the deck
        int capacity; // The maximum capacity of the deck
        Card cards[ TOTAL_CARDS ];
};

#endif