#include <algorithm>
#include <assert.h>
#include <iostream>
#include "card.hpp"
#include "hand.hpp"
using namespace std;

// Initializes a Hand with enough capacity to hold all but 1 of the cards in the game (one must always be face up).
// Note that the capacity is already set at compile time and this constructor merely initializes size and capacity.
// 
// PRE: none
// POST: size == 0; capacity == TOTAL_CARDS - 1; cards will remain of size TOTAL_CARDS - 1
Hand::Hand()
{
    size = 0;
    capacity = TOTAL_CARDS - 1;
}

// Prints the size, capacity, and comma-separated contents of the hand.
// 
// PRE: none
// POST: none
void
Hand::print() const
{
    cout << "Hand ( " << size << " / " << capacity << " cards): [";

    // If there is at least one card, special case it and print the others (if any)
    if ( size > 0 )
    {
        // Adding the space here rather than in the last print statement prevents a double space if the hand is empty
        cout << " " << cards[ 0 ].toStringShort();

        // Iterate over the hand and print each card separated by a comma and space
        for ( int i = 1; i < size; i++ )
        {
            cout << ", " << cards[ i ].toStringShort();
        }
    }

    cout << " ]";
}

// Prints just the contents of the hand, space-separated.
// 
// PRE: none
// POST: none
void
Hand::printContents() const
{
    // If the hand is empty, print nothing
    if ( isEmpty() )
    {
        return;
    }

    // Special case the first card and put a space before each subsequent card
    cout << cards[ 0 ].toStringShort();
    for ( int i = 1; i < size; i++ )
    {
        cout << " " << cards[ i ].toStringShort();
    }
}

// Appends the given card to the end of the hand (the first open slot).
// 
// PRE: hand must not be full
// POST: size will increase by 1; cards[ size - 1 ] == c
void
Hand::add( Card c )
{
    // Assert the preconditions
    assert( size < capacity );

    // If the hand is empty, just put the card in the first slot of the hand
    if ( size == 0 )
    {
        cards[ 0 ] = c;
        size++;
        return;
    }

    // Insert the card into the hand, effectively using one iteration of insertion sort
    int i = size;
    cards[ i ] = c;
    while ( i > 0 && cards[ i - 1 ].isGreaterThan( cards[ i ] ) )
    {
        swap( cards[ i ], cards[ i - 1 ] );
        i--;
    }

    // Increment the size as a card has been added
    size++;
}

// Returns the card at the given index.
// 
// PRE: 0 <= index < size
// POST: none
Card
Hand::getCardAt( int index ) const
{
    // Assert the preconditions
    assert( 0 <= index );
    assert( index < size );

    // If index is valid, simply return the value at it
    return cards[ index ];
}

// Returns the current size of the hand.
// 
// PRE: none
// POST: 0 <= return value <= capacity
int
Hand::getSize() const
{
    return size;
}

// Returns true if the hand is full (i.e. the size equals the capacity).
// 
// PRE: none
// POST: none
bool
Hand::isFull() const
{
    return size == capacity;
}

// Returns true if the hand is empty (i.e. the size equals 0).
// 
// PRE: none
// POST: none
bool
Hand::isEmpty() const
{
    return size == 0;
}

// Removes the card at the given index and shifts the others over to fill its place.
// 
// PRE: 0 <= index < size
// POST: size will decrease by 1; values after index will be moved to the next lower index
void
Hand::removeCardAt( int index )
{
    // Assert the preconditions
    assert( 0 <= index );
    assert( index < size );

    // Starting at the value after index, shift every card down, overwriting the previous one
    // The only card overwritten and not rewritten will be the value at index
    for ( int i = index + 1; i < size; i++ )
    {
        cards[ i - 1 ] = cards[ i ];
    }

    // Decrement size
    // There will be an unused duplicate value of cards[ size - 1 ] at cards[ size ] that should never be accessed
    // If there is only 1 card in the hand, the loop will be skipped but the size will decrease to 0, having the same effect
    size--;

    assert( isSorted() );
}

// Empties the hand by setting size to 0, making any previous contents inaccessible.
// 
// PRE: none
// POST: size == 0
void
Hand::clear()
{
    size = 0;
}

// Returns the first index of c in the hand.
// 
// PRE: c may be any card
// POST: return value will be -1 if c was not found
int
Hand::find( Card c ) const
{
    // Iterate across the hand, comparing each card with c
    for ( int i = 0; i < size; i++ )
    {
        // If this card is equal to c, return the index
        // Even if there are other cards equal to c, we only care about the first card
        if ( cards[ i ].isEqual( c ) )
        {
            return i;
        }
    }

    // If the loop completed without returning an index, c is not present, so return -1
    return -1;
}

// Returns the first index of a card whose toStringShort() matches the given string.
// 
// PRE: s may be any string
// POST: return value will be -1 if no matching card was found
int
Hand::findString( string s ) const
{
    // Iterate across the hand, comparing each card with c
    for ( int i = 0; i < size; i++ )
    {
        // If this card is equal to c, return the index
        // Even if there are other cards equal to c, we only care about the first card
        if ( s == cards[ i ].toStringShort() )
        {
            return i;
        }
    }

    // If the loop completed without returning an index, c is not present, so return -1
    return -1;
}

// Returns true if the hand is sorted in ascending order.
// 
// PRE: none
// POST: none
bool
Hand::isSorted() const
{
    // If the hand is empty or has only 1 card, it must be "sorted"
    if ( size <= 1 )
    {
        return true;
    }

    // Iterate across the hand, comparing each value to see if it is greater than the previous
    for ( int i = 1; i < size; i++ )
    {
        // If this card is not greater than or equal the previous, the hand is not sorted
        if ( cards[ i ].isLessThan( cards[ i - 1 ] ) )
        {
            return false;
        }
    }

    // If the loop completed without returning false, no out-of-place cards were found and the hand is sorted 
    return true;
}

// Sums the score of every card in the hand and returns this sum.
// 
// PRE: none
// POST: none
int
Hand::getScore() const
{
    // Sum the scores of cards in the hand
    int score = 0;
    for ( int i = 0; i < size; i++ )
    {
        score += cards[ i ].getScore();
    }

    return score;
}
