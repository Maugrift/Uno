#include <assert.h>
#include <iostream>
#include "deck.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "table.hpp"
using namespace std;

// Initializes a Player with the name "Player" and an empty hand.
// 
// PRE: none
// POST: name == "Player"; hand is a new, empty hand
Player::Player()
{
    name = "Player";
    score = 0;
    hand = Hand();
}

// Initializes a Player with a name and empty hand.
// 
// PRE: none
// POST: name == n; hand is a new, empty hand
Player::Player( string n )
{
    name = n;
    score = 0;
    hand = Hand();
}

// Returns the name of this player.
// 
// PRE: none
// POST: none
string
Player::getName() const
{
    return name;
}

// Returns this player's score.
// 
// PRE: none
// POST: return value >= 0
int
Player::getScore() const
{
    return score;
}

// Returns a reference to this player's hand.
// 
// PRE: none
// POST: none
Hand&
Player::getHand()
{
    return hand;
}

// Sets this player's score to the given value.
// 
// PRE: s >= 0
// POST: score == s
void
Player::setScore( int s )
{
    // Assert the preconditions
    assert( s >= 0 );

    score = s;
}

// Draws a card from the table.
// 
// PRE: table should have previously been initialized and should not be empty
// POST: return value is the card drawn
//       hand size will increase by one
//       the discard pile will be shuffled into the draw pile if the latter is empty
Card
Player::drawCard( Table& table )
{
    Card card = table.drawCard();
    hand.add( card );
    return card;
}

// Draws multiple cards from the table.
// 
// PRE: 0 <= nCards; table should have previously been initialized and should not be empty
// POST: hand size will increase by nCards; the discard pile will be shuffled into the draw pile if the latter is empty
void
Player::drawCards( int nCards, Table& table )
{
    // Assert the preconditions
    assert( nCards >= 0 );

    // If nCards == 0, this loop will be skipped and no cards will be drawn
    for ( int cardsDrawn = 0; cardsDrawn < nCards; cardsDrawn++ )
    {
        hand.add( table.drawCard() );
    }
}

// Plays the card at the given index on the given table.
// 
// PRE: 0 <= cardIndex < hand size; the card must be playable on the table's top card
// POST: hand size will decrease by 1
void
Player::playCardIndex( int cardIndex, Table& table, int wildColor )
{
    // Assert the preconditions
    assert( cardIndex >= 0 );
    assert( cardIndex < hand.getSize() );

    Card card = hand.getCardAt( cardIndex );

    // Play the card on the table and remove it from the hand
    // Table will assert that this card is playable on its top card
    table.playCard( card, wildColor );
    hand.removeCardAt( cardIndex );
}

// Plays the card at the given index on the given table.
// 
// PRE: the card must be in the hand; the card must be playable on the table's top card
// POST: hand size will decrease by 1
void
Player::playCard( Card card, Table& table, int wildColor )
{
    // Find the index of the card
    int cardIndex = hand.find( card );
    
    // Assert that the card is valid
    assert( cardIndex != -1 );

    // Play the card on the table and remove it from the hand
    // Table will assert that this card is playable on its top card
    table.playCard( card, wildColor );
    hand.removeCardAt( cardIndex );
}
