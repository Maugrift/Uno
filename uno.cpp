// uno.cpp - Simulates the card game Uno.
//
// Author: Aaron Friesen
//
// Date: Wed 19 Jun 2019
//
// Time Spent: 13 hours 30 minutes
// 
// Description: Implementation works as specified with the only omission being the restriction on playing Draw4s.

#include <cstdlib>
#include <iostream>
#include <time.h>
#include "card.hpp"
#include "deck.hpp"
#include "game.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "table.hpp"
using namespace std;

void printInstructions();

int main()
{
    // Seed the random number generator (necessary for shuffling the deck)
    srand( time( 0 ) );

    ////////////////////////////////////////////////////////////////////////////////
    // INITIAL INPUT
    ////////////////////////////////////////////////////////////////////////////////

    // Junk variable used to consume "enter to continue" input or trailing newlines
    string junk;

    // Print the name of the game and prompt to show instructions
    cout << "Welcome to UNO!" << endl;
    cout << "Show instructions? (y/N) ";
    string input;
    getline( cin, input );

    // If y is entered, print the instructions
    if ( input == "y" || input == "Y" )
    {
        printInstructions();
    }

    // Prompt for the number of players
    cout << endl;
    int nPlayers;
    do
    {
        cout << "Enter the number of players ( 2-6 ): ";
        cin >> nPlayers;
        if ( !( nPlayers >= 2 ) )
        {
            cout << "Number of players ( " << nPlayers << " ) must be at least 2." << endl;
        }
        if ( !( nPlayers <= MAX_PLAYERS ) )
        {
            cout << "Number of players ( " << nPlayers << " ) must be at most " << MAX_PLAYERS << "." << endl;
        }
    } while ( cin && !( nPlayers >= 2 && nPlayers <= MAX_PLAYERS ) );

    // If cin is in a fail state, exit the program
    if ( !cin )
    {
        exit( 1 );
    }

    // Consume the trailing newline from cin
    getline( cin, junk );

    // Prompt for the names of each player and initialize the players array
    string names[ nPlayers ];
    for ( int i = 0; i < nPlayers; i++ )
    {
        string name;
        cout << "Enter the name of Player " << i + 1 << ": ";
        getline( cin, name );
        names[ i ] = name;
    }

    // Prompt for the number of points to play to
    int goalScore;
    do
    {
        cout << "Enter the number of points to play to (a standard game is 500): ";
        cin >> goalScore;
        if ( !( goalScore > 1 ) )
        {
            cout << "Points ( " << goalScore << " ) must be at least 1." << endl;
        }
    } while ( cin && !( goalScore > 0 ) );

    // If cin is in a fail state, exit the program
    if ( !cin )
    {
        exit( 1 );
    }

    // Consume the trailing newline
    getline( cin, junk );

    ////////////////////////////////////////////////////////////////////////////////
    // GAMEPLAY
    ////////////////////////////////////////////////////////////////////////////////

    // Initialize the Game object
    Game game( names, nPlayers );

    // Game loop (each iteration is a round)
    bool endGame = false;
    int round = 1;
    while ( !endGame )
    {
        // Print the round number
        cout << endl;
        cout << "<<< ROUND " << round << " >>>" << endl;

        // Initialize the Game for a new round
        // This may trigger input and card effects when the stock's action is processed
        game.initializeRound();

        // Round loop (each iteration is a turn)
        bool endRound = false;
        while ( !endRound )
        {
            // Print information for the current player, get their input, and process their turn
            cout << endl;
            game.printTurnHeader();
            game.processPlayerTurn();

            // If the round is over, exit the loop
            if ( game.roundIsOver() )
            {
                endRound = true;
            }
            // Otherwise, continue to the next player's turn
            else
            {
                game.nextPlayer();
            }
        }

        // Score the round
        game.scoreRound();

        // Print the scores of each player
        cout << endl;
        cout << "Round " << round << " Scores:" << endl;
        game.printScores();

        // If this player has won the game, print a message and end the game
        Player& winner = game.getRoundWinner();
        cout << endl;
        if ( winner.getScore() >= goalScore )
        {
            endGame = true;
            cout << winner.getName() << " has won the game!" << endl;
        }
        // If not, print a message and continue to the next round
        else
        {
            cout << winner.getName() << " has won Round " << round << "!" << endl;
            round++;

            cout << "Press enter to continue to round " << round << ".";
            getline( cin, junk );
        }
    }

    // The game has ended, meaning someone has won, so exit the program
    return 0;
}

// Prints the instructions of the game, waiting for the player to press enter between each paragraph.
// 
// PRE: none
// POST: none
void printInstructions()
{
    // Used to consume input from "enter to continue" prompts
    string junk;

    cout << endl;
    cout << "Uno is a popular card game that is closely related to the classic game Crazy Eights." << endl;
    cout << "It falls into the \"get rid of all of your cards\" genre of games." << endl;
    cout << "Uno is played with a deck of 108 cards." << endl;
    cout << "There are a few basic categories of cards:" << endl;
    cout << "  Number cards: (ranked 0−9), which may be of one of four colors: Red (r), Yellow (y), Green (g), or Blue (b).";
    cout << "  Action cards: (Draw2, Reverse, Skip), which may be of any of the aforementioned colors." << endl;
    cout << "  Wild cards in two varieties − the \"plain\" Wild card, and the Draw4 Wild Card." << endl;
    cout << "There is one 0 card of each color, and two of each card 1−9 for each color (total of 76 cards)." << endl;
    cout << "There are also two of each action card for each color (24 cards)." << endl;
    cout << "There are 4 plain Wild cards and 4 Draw4 Wild cards." << endl;
    cout << "(Press enter to continue.)";
    getline( cin, junk );
    cout << endl;
    cout << "To start the game, the deck is shuffled, and each player is dealt a hand of seven cards." << endl;
    cout << "The top card of the deck (called the \"stock\") is revealed and placed to being the discard pile." << endl;
    cout << "If the card is a Draw4 Wild Card, then the card is returned to the deck, the deck is reshuffled and a new card is revealed." << endl;
    cout << "This is done until the card revealed is NOT a Draw4 Wild card." << endl;
    cout << "The card is treated as if the dealer played the card, so any action cards (Draw2, Reverse, Skip, Wild) will have their stated effect." << endl;
    cout << "Unless the first card is a Reverse card, play proceeds in the given player order." << endl;
    cout << "If the first card is a Reverse card, play starts in reverse player order." << endl;
    cout << "(Press enter to continue.)";
    getline( cin, junk );
    cout << endl;
    cout << "On a player's turn, a player MAY play a card from her hand OR draw a card from the deck." << endl;
    cout << "If a player chooses to draw a card from the deck, she MAY choose to play the drawn card, if it is a legal play." << endl;
    cout << "If a player has no legal play, then she must draw a card." << endl;
    cout << "(Press enter to continue.)";
    getline( cin, junk );
    cout << endl;
    cout << "A card may be played if it matches the color of the top (i.e. most recently discarded) card on the discard pile, OR if it matches the the number or action of the top card on a discard pile." << endl;
    cout << "For example, if the top card of the discard pile is a Red 5 (r5), then any Red card or any 5 may be played." << endl;
    cout << "In addition, a plain Wild card may be played regardless of the top card of the discard." << endl;
    cout << "When playing a Wild card, the player chooses what color the Wild card will be for the purposes of the next play." << endl;
    cout << "A Draw4 Wild card may only be played if the player has no cards that match the color of the top card." << endl;
    cout << "That is, if the top card is a Red 5, then to play a Draw4 Wild card, a player must have no Red cards in her hand." << endl;
    cout << "(Press enter to continue.)";
    getline( cin, junk );
    cout << endl;
    cout << "The effects of the action cards are pretty straightforward:" << endl;
    cout << "  Draw2 (D) − the next player must draw two cards before taking their turn." << endl;
    cout << "  Reverse (R) − the direction of play is reversed." << endl;
    cout << "  Skip (S) − the player that would ordinarily be next is skipped." << endl;
    cout << "  Wild (W) − this card may always be played; the player of this card announces what color it becomes for the purposes of the next play." << endl;
    cout << "  Draw4 Wild (X) − may only be played when no cards of the same color are held; the player announces what color it becomes for purposes of the next play, and the next player must draw 4 cards before taking her turn." << endl;
    cout << "(Press enter to continue.)";
    getline( cin, junk );
    cout << endl;
    cout << "When a player has played her last card, then she receives points according the cards remaining in the hands of the other players." << endl;
    cout << "Number cards are worth points equal to their face value." << endl;
    cout << "Action cards (Draw2, Reverse, Skip) are worth 20 points each, and Wild and Draw4 Wild cards are worth 50 points." << endl;
    cout << "(Press enter to continue.)";
    getline( cin, junk );
}
