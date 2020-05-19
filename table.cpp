#include <assert.h>
#include <iostream>
#include "card.hpp"
#include "table.hpp"
using namespace std;

// Initializes a Table, constructing but not initializing its 2 decks.
// 
// PRE: none
// POST: draw and discard will be empty
Table::Table()
{
    draw = Deck();
    discard = Deck();
}

// Initialize the draw and discard piles.
// The draw pile will be initialized and shuffled, the discard pile will be emptied,
// and cards will be placed from the draw pile onto the discard pile until the top card is not a Draw4 Wild.
// 
// PRE: none
// POST: draw will have at most TOTAL_CARDS - 1 cards
//       discard will have at least 1 card
//       the top card of the discard will not be a Draw4 Wild
void
Table::initialize()
{
    // Initialize the decks
    draw.initialize();
    draw.shuffle();
    discard.clear();

    // Put the top card of the deck on the discard pile
    // Repeat until the top card is not a Draw4 Wild
    do
    {
        discard.push( draw.pop() );
    } while ( discard.peek().getValue() == DRAW4_WILD_INDEX );
}

// Returns the total number of cards on the table.
// 
// PRE: none
// POST: none
int
Table::getTotalCards() const
{
    return draw.getSize() + discard.getSize();
}

// Returns true if a card can be drawn from the table.
// 
// PRE: none
// POST: none
bool
Table::canDrawCard() const
{
    return getTotalCards() > 1;
}

// Returns true if the given number of cards can be drawn from the table.
// 
// PRE: none
// POST: none
bool
Table::canDrawCards( int nCards ) const
{
    return getTotalCards() > nCards;
}

// Pops the top card of the draw pile, shuffling the discard pile to replace it if necessary.
// 
// PRE: the table must not be empty
// POST: if the draw pile was empty, the discard pile will become the new draw pile, excluding its top card
Card
Table::drawCard()
{
    // Assert the preconditions
    assert( canDrawCard() );

    // If the deck is empty, reshuffle the discard pile
    // This is done by swapping the two decks (except for the top card)
    if ( draw.isEmpty() )
    {
        // Keep the top card in play by swapping it to the draw pile (the new discard pile)
        draw.push( discard.pop() );

        // Swap the draw and discard piles
        // It may be possible to optimize this operation with references
        Deck swap = draw;
        draw = discard;
        discard = swap;

        // Shuffle the new draw pile and print a message
        draw.shuffle();
    }

    // Add the top card of the draw pile to the player's hand
    return draw.pop();
}

// Plays the given card onto the top of the discard pile.
// 
// PRE: the given card must be playable on the top card of the discard pile
// POST: none
void
Table::playCard( Card card, int wildColor )
{
    // Assert the preconditions
    assert( card.canPlayOn( discard.peek(), wildColor ) );

    discard.push( card );
}

// Returns the top card of the discard pile.
// 
// PRE: discard must not be empty (should not occur if table has been initialized)
// POST: none (discard will not be affected)
Card
Table::getStock() const
{
    // Assert the preconditions
    assert( !discard.isEmpty() );

    return discard.peek();
}
