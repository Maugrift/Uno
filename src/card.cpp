#include <assert.h>
#include <iostream>
#include "card.hpp"
using namespace std;

// Initializes a Card as a Red 0.
// 
// PRE: none
// POST: color == 0; value == 0
Card::Card()
{
    color = 0;
    value = 0;
}

// Initializes a Card of the given color and value.
// 
// PRE: 0 <= c <= 4; 0-3 are actual colors, 4 represents cards with no color (i.e. unplayed wild cards)
//      0 <= v <= 14; 0-9 are number cards, 10-14 are action/wild cards
//      if c == 4 (no color), v == 13 or 14 (wild or draw4 wild)
// POST: color == c; value == v
Card::Card( int c, int v )
{
    // Assert the preconditions
    assert( c >= 0 );
    assert( c <= 4 );
    assert( v >= 0 );
    assert( v <= 14 );
    if ( c == 4 )
    {
        assert( v == 13 || v == 14 );
    }

    color = c;
    value = v;
}

// Returns the integer color of the card.
// 
// PRE: none
// POST: 0 <= return value < N_COLORS, or return value == NO_COLOR_INDEX
int
Card::getColor() const
{
    return color;
}

// Returns the integer value of the card.
// 
// PRE: none
// POST: 0 <= return value < N_VALUES
int
Card::getValue() const
{
    return value;
}

// Returns the full name of the card's color (e.g. Red). If the card is a wild card, returns "None".
// 
// PRE: none
// POST: none
string
Card::getColorAsString() const
{
    return COLOR_STRINGS[ color ];
}

// Returns the full name of the card's value (e.g. Draw2).
// For number cards, this is just the numeral value as a string.
// 
// PRE: none
// POST: none
string
Card::getValueAsString() const
{
    return VALUE_STRINGS[ value ];
}

// Returns the abbreviated version of the card (e.g. rD).
// 
// PRE: none
// POST: none
string
Card::toStringShort() const
{
    const char shortName[] = { COLOR_CHARS[ color ], VALUE_CHARS[ value ] }; 
    string nameString( shortName, 2 );
    return nameString;
}

// Returns the full name of the card (e.g. Red Draw2).
// 
// PRE: none
// POST: none
string
Card::toStringLong() const
{
    // If the card is a wild card, just return its value string
    if ( isWild() )
    {
        return getValueAsString();
    }

    return getColorAsString() + " " + getValueAsString();
}

// Prints the abbreviated version of the card (e.g. rD).
// 
// PRE: none
// POST: none
void
Card::printShort() const
{
    cout << toStringShort();
}

// Prints the full name of the card (e.g. Red Draw2).
// 
// PRE: none
// POST: none
void
Card::printLong() const
{
    cout << toStringLong();
}

// Returns the point value of the card.
// 
// PRE: none
// POST: none
int
Card::getScore() const
{
    // All wild cards have the same value
    if ( isWild() )
    {
        return WILD_SCORE;
    }

    // All action cards have the same value
    if ( isAction() )
    {
        return ACTION_SCORE;
    }

    // Number cards are worth their face value
    return value;
}

// Returns true if this card is an action card.
// In this implementation, wild cards are not considered action cards.
// 
// PRE: none
// POST: none
bool
Card::isAction() const
{
    return value >= 10 && value <= 12;
}

// Returns true if this card is a wild card.
// 
// PRE: none
// POST: none
bool
Card::isWild() const
{
    return value >= 13;
}

// Returns true if this card can be played on the given card.
// 
// PRE: if the card is a wild card, wildColor must be valid (0 <= wildColor < N_COLORS)
// POST: none
bool
Card::canPlayOn( Card other, int wildColor ) const
{
    // If this card is wild, it can be played on anything
    if ( isWild() )
    {
        return true;
    }

    // If the other card is wild and the colors match, this card can be played on it
    if ( other.isWild() )
    {
        // Assert that wildColor is valid
        // It should not be checked before this because a wild card might not have been played yet
        assert( wildColor >= 0 );
        assert( wildColor < N_COLORS );

        return color == wildColor;
    }

    // If at least one attribute is shared between the cards, this card can be played on the other
    return color == other.color || value == other.value;
}

// Returns true if this card equals the given card.
// 
// PRE: none
// POST: none
bool
Card::isEqual( Card other ) const
{
    return color == other.color && value == other.value;
}

// Returns true if this card is less than the given card.
// Cards are first sorted by color alphabetically and then value.
// 
// PRE: none
// POST: none
bool
Card::isLessThan( Card other ) const
{
    if ( color < other.color )
    {
        return true;
    }
    
    if ( color == other.color )
    {
        return value < other.value;
    }
    
    return false;
}

// Returns true if this card is greater than the given card.
// Cards are first sorted by color alphabetically and then value.
// 
// PRE: none
// POST: none
bool
Card::isGreaterThan( Card other ) const
{
    if ( color > other.color )
    {
        return true;
    }
    
    if ( color == other.color )
    {
        return value > other.value;
    }
    
    return false;
}
