#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include "card.hpp"
#include "deck.hpp"
using namespace std;

// Initializes a Deck with exactly enough capacity to hold every card in the game.
// Note that the capacity is already set at compile time and this constructor merely initializes size and capacity.
// 
// PRE: none
// POST: size == 0; capacity == TOTAL_CARDS; cards will remain of size TOTAL_CARDS
Deck::Deck()
{
    size = 0;
    capacity = TOTAL_CARDS;
}

// Prints the size, capacity, and contents of the deck.
// 
// PRE: none
// POST: none
void
Deck::print() const
{
    cout << "Deck ( " << size << " / " << capacity << " cards): [";

    // If there is at least one card, special case it and print the others (if any)
    if ( size > 0 )
    {
        // Adding the space here rather than in the last print statement prevents a double space if the deck is empty
        cout << " " << cards[ 0 ].toStringShort();

        // Iterate over the deck and print each card separated by a comma and space
        for ( int i = 1; i < size; i++ )
        {
            cout << ", " << cards[ i ].toStringShort();
        }
    }

    cout << " ]";
}

// Initializes the deck with the correct number of each card. For these numbers, see card.hpp.
// 
// PRE: none
// POST: the deck will be filled and any previous contents will be overwritten
void
Deck::initialize()
{
    // Empty the deck by setting size to 0
    size = 0;

    // Add the cards of each color
    for ( int colorIndex = 0; colorIndex < N_COLORS; colorIndex++ )
    {
        // Add 0-value number cards
        for ( int duplicates = 0; duplicates < N_0_CARDS; duplicates++ )
        {
            push( Card( colorIndex, FIRST_NUMBER_INDEX ) );
        }

        // Add number cards 1-9
        for ( int valueIndex = FIRST_NUMBER_INDEX + 1; valueIndex <= LAST_NUMBER_INDEX; valueIndex++ )
        {
            for ( int duplicates = 0; duplicates < N_NUMBER_CARDS; duplicates++ )
            {
                push( Card( colorIndex, valueIndex ) );
            }
        }

        // Add action cards
        for ( int valueIndex = FIRST_ACTION_INDEX; valueIndex <= LAST_ACTION_INDEX; valueIndex++ )
        {
            for ( int duplicates = 0; duplicates < N_ACTION_CARDS; duplicates++ )
            {
                push( Card( colorIndex, valueIndex ) );
            }
        }
    }

    // Add wild cards
    for ( int valueIndex = FIRST_WILD_INDEX; valueIndex <= LAST_WILD_INDEX; valueIndex++ )
    {
        for ( int duplicates = 0; duplicates < N_WILD_CARDS; duplicates++ )
        {
            push( Card( NO_COLOR_INDEX, valueIndex ) );
        }
    }
}

// Adds the given card to the top of the deck (the slot after the last card in the array).
// 
// PRE: deck must not be full
// POST: size will increase by 1; cards[ size - 1 ] == c
void
Deck::push( Card c )
{
    // Assert the preconditions
    assert( size < capacity );

    // The size of the deck is also the index of the first empty slot
    // Thus we can use this index to append c and increment the size
    cards[ size ] = c;
    size++;
}

// Returns and removes the top card of the deck (the last card in the array).
// 
// PRE: deck must not be empty
// POST: size will decrease by 1
Card
Deck::pop()
{
    // Assert the preconditions
    assert( size > 0 );

    // Decrement size, leaving the card untouched but inaccessible through class methods
    // Normally the last card is at the index size - 1, but since size has decreased by 1, the last card is at size
    size--;
    return cards[ size ];
}

// Returns but does not remove the top card of the deck (the last card in the array).
// 
// PRE: deck must not be empty
// POST: none
Card
Deck::peek() const
{
    // Assert the preconditions
    assert( size > 0 );

    // Return the top card
    return cards[ size - 1 ];
}

// Returns the current size of the deck.
// 
// PRE: none
// POST: none
int
Deck::getSize() const
{
    return size;
}

// Returns true if the deck is full (i.e. the size equals the capacity).
// 
// PRE: none
// POST: none
bool
Deck::isFull() const
{
    return size == capacity;
}

// Returns true if the deck is empty (i.e. the size equals 0).
// 
// PRE: none
// POST: none
bool
Deck::isEmpty() const
{
    return size == 0;
}

// Empties the deck by setting its size to 0, making any previous contents inaccessible.
// 
// PRE: none
// POST: size == 0
void
Deck::clear()
{
    size = 0;
}

// Randomizes the order of cards in the deck.
// 
// PRE: the random number generator should be seeded before calling shuffle()
// POST: none
void
Deck::shuffle()
{
    // Iterate over the deck, swapping each card with another random card in the deck
    for ( int i = 0; i < size; i++ )
    {
        swap( cards[ i ], cards[ rand() % size] );
    }
}
