#include <assert.h>
#include <iostream>
#include "game.hpp"
using namespace std;

// Initializes a Game with the given players and goal score.
// 
// PRE: p should be of size nP
//      2 <= nP <= MAX_PLAYERS
//      gS >= 1
// POST: 
Game::Game( string playerNames[], int nPlayers )
{
    // Assert the preconditions
    assert( nPlayers >= 2 );
    assert( nPlayers <= MAX_PLAYERS );

    // Foo* pFoo = new Foo();
    // (*pFoo).counter ++;
    // pFoo->counter++;

    // Initialize nPlayers
    this->nPlayers = nPlayers;
    
    // Copy players to the players array
    for ( int playerIndex = 0; playerIndex < nPlayers; playerIndex++ )
    {
        players[ playerIndex ] = Player( playerNames[ playerIndex ] );
    }
}

// Prompts for a valid color (red, yellow, green, or blue) for a wild card.
// 
// PRE: none
// POST: 0 <= return value <= 3
int
Game::getColorInput() const
{
    // Prompt until a valid color is entered, at which point the function will return
    while (true)
    {
        // Prompt for the color of the wild card
        cout << "Choose a color for your wild card (r, y, g, b): ";
        string color;
        getline( cin, color);
        if ( color.size() == 1 )
        {
            // If just one character was entered, get the int corresponding to the character
            // It may better to iterate over the colors array, but this would probably be slower
            switch ( color.at( 0 ) )
            {
                case 'r':
                    return 0;
                case 'y':
                    return 1;
                case 'g':
                    return 2;
                case 'b':
                    return 3;
                default:
                    // A character other than r, y, g, or b was entered, so print an error and re-prompt
                    cout << "Please enter r, y, g, or b." << endl;
                    break;
            }
        }
        else
        {
            // More than one character was entered, so print an error and re-prompt
            cout << "Please enter just one character (r, y, g, or b)." << endl;
        }
    }
}

// Initializes the game for a round.
// 
// PRE: none
// POST: table, currentPlayerIndex, reverse, skip, and wildColor will be initialized
//       all players' hands will be cleared and they will be dealt new cards
void
Game::initializeRound()
{
    // Initialize fields
    table.initialize();
    currentPlayerIndex = 0;
    reverse = false;
    skip = false;
    wildColor = NO_COLOR_INDEX;

    // Empty each player's hand
    for ( int playerIndex = 0; playerIndex < nPlayers; playerIndex++ )
    {
        players[ playerIndex ].getHand().clear();
    }

    // Deal cards to each player
    for ( int card = 0; card < STARTING_HAND_SIZE; card++ )
    {
        for ( int playerIndex = 0; playerIndex < nPlayers; playerIndex++ )
        {
            players[ playerIndex ].drawCard( table );
        }
    }

    // Apply the effects of the stock to the first player
    // Optimally, processCardAction() would be used for this, but it depends on other variables initialized in this function
    Player& firstPlayer = players[ 0 ];
    switch ( table.getStock().getValue() )
    {
        // First player draws 2 cards
        case DRAW2_INDEX:
            firstPlayer.drawCards( 2, table );
            cout << endl;
            cout << "The first stock is a Draw2, so " << firstPlayer.getName() << " draws 2 cards." << endl;
            break;
        // Play is reversed following the first player's turn
        case REVERSE_INDEX:
            reverse = !reverse;
            cout << endl;
            cout << "The first stock is a Reverse, so the direction of play starts reversed." << endl;
            break;
        // First player is skipped
        case SKIP_INDEX:
            skip = true;
            cout << endl;
            cout << "The first stock is a Skip, so " << firstPlayer.getName() << " is skipped." << endl;
            break;
        // First player may choose the color of the Wild card
        case WILD_INDEX:
            cout << endl;
            cout << "The first stock is a Wild card, so " << firstPlayer.getName() << " will pick its color." << endl;
            cout << "Your Hand: ";
            firstPlayer.getHand().printContents();
            cout << endl;
            wildColor = getColorInput();
            break;
    }
}

// Returns the player who will take their turn next.
// 
// PRE: the round should have been initialized
// POST: none
int
Game::getNextPlayerIndex() const
{
    // Determine the player increment (the difference between the current player's index and the next player's)
    int playerIncrement = reverse ? -1 : 1;
    int nextPlayerIndex = currentPlayerIndex + playerIncrement;
    if ( skip )
    {
        // To skip the player who would normally be next, just increment one more time
        nextPlayerIndex += playerIncrement;
    }

    // If nextPlayerIndex is negative, manually wrap it by adding nPlayers
    if ( nextPlayerIndex < 0 )
    {
        while ( nextPlayerIndex < 0 )
        {
            nextPlayerIndex += nPlayers;
        }
    }
    // If nextPlayerIndex is positive, simply wrap it with a mod operation
    else
    {
        nextPlayerIndex %= nPlayers;
    }

    return nextPlayerIndex;
}

// Sets the current player to the next player in the turn sequence.
// 
// PRE: round should be initialized
// POST: skip will be set to false
void
Game::nextPlayer()
{
    currentPlayerIndex = getNextPlayerIndex();
    skip = false;
}


// Prints a header for the current player, containing pertinent information for their turn.
// 
// PRE: round should be initialized
// POST: none
void
Game::printTurnHeader() const
{
    // Define convenience variables (no references)
    Player player = players[ currentPlayerIndex ];

    // Print whose turn it is and who the next player is
    cout << "*** " << player.getName() << "'s Turn ***" << endl;
    cout << "Next Player: " << players[ getNextPlayerIndex() ].getName() << endl;

    // Print the number of cards each player has remaining
    cout << "Cards Remaining:";
    for ( int i = 0; i < nPlayers; i++ )
    {
        if ( i != currentPlayerIndex )
        {
            Player p = players[ i ];
            cout << " " << p.getHand().getSize() << " ( " << p.getName() << " )";
        }
    }
    cout << endl;

    // Print the stock and its color if it's wild
    Card stock = table.getStock();
    cout << "Stock: " << stock.toStringLong() << " ( " << stock.toStringShort() << " )";
    if ( stock.isWild() )
    {
        cout << " ( " << COLOR_STRINGS[ wildColor ] << " )";
    }
    cout << endl;

    // Print the current player's hand and its contents
    cout << "Your Hand: ";
    player.getHand().printContents();
    cout << endl;
}

// Draws a card for the current player, if possible, and prompts them if they want to play it.
// 
// PRE: the round should be initialized
// POST: the current player will draw a card from the table, if possible, and potentially play it
void
Game::drawCard()
{
    // Define convenience variables
    Player& player = players[ currentPlayerIndex ];

    // If possible, draw a card
    if ( table.canDrawCard() )
    {
        Card card = player.drawCard( table );
        cout << "You drew a " << card.toStringLong() + "." << endl;

        // If the player can play the card, prompt to see if they want to play it (default is yes)
        if ( card.canPlayOn( table.getStock(), wildColor ) )
        {
            cout << "Play it? (Y/n) ";
            string input;
            getline( cin, input );

            // If the player chooses to play the card, play it
            if ( input != "n" && input != "N" )
            {
                player.playCard( card, table, wildColor );
                processCardAction( card );
            }
        }
    }
    // If the table is empty, the player won't be able to draw a card, so print a message
    else
    {
        cout << endl;
        cout << "The draw and discard piles are empty, so your turn is skipped." << endl;
    }
}

// Prompts for a valid card for the current player to play.
// 
// PRE: the round should be initialized
// POST: return value will be a valid card for the current player to play
//       the game will not change
Card
Game::getCardInput() const
{
    // Define convenience variables (no references)
    Player player = players[ currentPlayerIndex ];
    Hand hand = player.getHand();
    Card stock = table.getStock();

    // Will continue until valid input is received, upon which the method will return
    while ( true )
    {
        // Prompt the player for the card to play
        string cardString;
        cout << "Choose a card to play: ";
        getline( cin, cardString );
        int cardIndex = player.getHand().findString( cardString );

        // The card was not found in the player's hand, so print an error
        if ( cardIndex == -1 )
        {
            cout << "You do not have the card \"" << cardString << "\" in your hand." << endl;
            cout << "Enter one of the cards in your hand, as listed above." << endl;
            cout << endl;
        }
        // The player entered a valid card, so check if it can be played
        else
        {
            Card card = hand.getCardAt( cardIndex );
            
            // If this card cannot be played on the stock, it is not valid
            if ( !card.canPlayOn( stock, wildColor ) )
            {
                cout << "You cannot play a " << card.toStringLong() << " on a " << stock.toStringLong() << "." << endl;
                cout << "Either the color or the value must match." << endl;
                cout << endl;
            }
            // This card is valid, so return it
            else
            {
                return card;
            }
        }
    }
}

// Makes the given player draw up to the given number of cards and print a message.
// 
// PRE: nCards >= 0; round should be initialized
// POST: the given player will draw at most the given number of cards
void
Game::drawUpTo( Player& player, int nCards )
{
    assert( nCards >= 0 );

    // If the number of cards to draw is 0, return without drawing anything
    if ( nCards == 0 )
    {
        return;
    }

    // If the number of cards on the table is less than the number of cards specified, draw all the cards on the table
    int maxCards = min( table.getTotalCards() - 1, nCards );
    player.drawCards( maxCards, table );

    // Print a message corresponding to the number of cards drawn
    if ( maxCards == 0 )
    {
        cout << "The table is empty, so " << player.getName() << " draws no cards." << endl;
    }
    else if ( maxCards == 1 )
    {
        if ( nCards == 1 )
        {
            cout << player.getName() << " draws 1 card." << endl;
        }
        else
        {
            cout << player.getName() << " draws 1 card, but there are not enough cards on the table to draw up to " << nCards << "." << endl;
        }
    }
    else if ( maxCards < nCards )
    {
        cout << player.getName() << " draws " << maxCards << " cards, but there are not enough cards on the table to draw up to " << nCards << "." << endl;
    }
    else
    {
        cout << player.getName() << " draws " << nCards << " cards." << endl;
    }
}

// Processes the action of the given card as if the current player played it.
// 
// PRE: round should be initialized
// POST: 
void
Game::processCardAction( Card card )
{
    // If the card is an action card, process its effect and print a message
    // maxCards and nextPlayer must be initialized here, otherwise the jump to later case labels crosses their initialization
    Player& nextPlayer = players[ getNextPlayerIndex() ] ;
    switch ( card.getValue() )
    {
        // Make the next player draw 2 cards
        // The official rules say that this also skips the next player, but the spec does not mention this
        case DRAW2_INDEX:
            drawUpTo( nextPlayer, 2 );
            break;
        // Reverse the direction of play
        case REVERSE_INDEX:
            reverse = !reverse;
            cout << "The direction of play has been reversed." << endl;
            break;
        // Skip the next player
        case SKIP_INDEX:
            skip = true;
            cout << nextPlayer.getName() << " is skipped." << endl;
            break;
        // Choose a color
        case WILD_INDEX:
            wildColor = getColorInput();
            break;
        // Choose a color and make the next player draw 4 cards
        // The official rules say that this also skips the next player, but the spec does not mention this
        case DRAW4_WILD_INDEX:
            wildColor = getColorInput();
            drawUpTo( nextPlayer, 4 );
    }
}

// Processes the current player's turn, including input, drawing, and playing.
// 
// PRE: round should be initialized
// POST: the player will either:
//       -draw,
//       -draw and play the drawn card,
//       -fail to draw (because the table is empty), or
//       -play from their hand
void
Game::processPlayerTurn()
{
    // Define convenience variables
    Player& player = players[ currentPlayerIndex ];
    Hand& hand = player.getHand();
    string junk;

    // Check to see if the player can play any card
    bool canPlay = false;
    for ( int cardIndex = 0; cardIndex < player.getHand().getSize(); cardIndex++ )
    {
        if ( hand.getCardAt( cardIndex ).canPlayOn( table.getStock(), wildColor ) )
        {
            canPlay = true;
            break;
        }
    }

    // If the player cannot play, automatically draw for them
    if ( !canPlay )
    {
        cout << "You have no plays available. Press enter to draw a card.";
        getline( cin, junk );
        drawCard();
    }
    // If the player can play, prompt to see if they want to draw
    else
    {
        cout << "Draw a card? (y/N) ";
        string input;
        getline( cin, input );

        // If the player chooses to draw a card, they will draw and not play from their hand
        if ( input == "y" || input == "Y" )
        {
            drawCard();
        }
        // If the player chooses not to draw, they will choose a card from their hand to play
        else
        {
            // Prompt for a valid card to play
            Card card = getCardInput();
            
            // Play the card
            player.playCard( card, table, wildColor );

            // Print a message for other players to reference
            cout << endl;
            cout << player.getName() << " plays a " << card.toStringLong() << "." << endl;
            
            // Process the effect of the card, if any
            processCardAction( card );
        }
    }
}

// Returns true if the round is over, i.e. one player has no cards in their hand.
// 
// PRE: none
// POST: none
bool
Game::roundIsOver() const
{
    // Iterate through each player
    for ( int playerIndex = 0; playerIndex < nPlayers; playerIndex++ )
    {
        // If this player's hand is empty, they have won the round
        Player player = players[ playerIndex ];
        if ( player.getHand().isEmpty() )
        {
            return true;
        }
    }

    return false;
}

// Returns the winner of the current round.
// 
// PRE: the round must be over
// POST: none
Player&
Game::getRoundWinner()
{
    // Assert the preconditions
    assert( roundIsOver() );

    // Iterate through each player
    for ( int playerIndex = 0; playerIndex < nPlayers; playerIndex++ )
    {
        // If this player's hand is empty, they have won the round
        Player& player = players[ playerIndex ];
        if ( player.getHand().isEmpty() )
        {
            return player;
        }
    }

    // Because the round is over, this should not be reached
    assert( false );
}

// Increases the winner's score by the sum of their opponents' cards.
// 
// PRE: the round must be over
// POST: none
void
Game::scoreRound()
{
    // Assert the preconditions
    assert( roundIsOver() );

    Player& winner = getRoundWinner();

    // Iterate through each player and add their hand's score to the round score
    // The winner's score will be 0, so it doesn't matter that their hand is included
    int roundScore = 0;
    for ( int playerIndex = 0; playerIndex < nPlayers; playerIndex++ )
    {
        roundScore += players[ playerIndex ].getHand().getScore();
    }

    // Increase the winner's score
    winner.setScore( winner.getScore() + roundScore );
}

// Prints the scores of each player, sorted in descending order.
// 
// PRE: none
// POST: none
void
Game::printScores() const
{
    // Create a ranks array and populate it with each possible player index in ascending order
    // The players array cannot be changed as its order determines the turn order
    int ranks[ nPlayers ];
    for ( int i = 0; i < nPlayers; i++ )
    {
        ranks[ i ] = i;
    }

    // Sort the ranks array using insertion sort
    for ( int i = 0; i < nPlayers; i++ )
    {
        int j = i;
        while ( j > 0 && players[ ranks[ j - 1 ] ].getScore() > players[ ranks[ j ] ].getScore() )
        {
            int swap = ranks[ j ];
            ranks[ j ] = ranks[ j - 1 ];
            ranks[ j - 1 ] = swap;
            j--;
        }
    }

    // Print the players and their scores from highest (best) to lowest (worst) score
    for ( int rank = nPlayers - 1; rank >= 0; rank-- )
    {
        Player player = players[ ranks[ rank ] ];
        cout << rank + 1 << ". " << player.getName() << " ( " << player.getScore() << " )" << endl;
    }
}
