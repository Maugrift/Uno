#ifndef CARD
#define CARD

using namespace std;

const char COLOR_CHARS[] = { 'r', 'y', 'g', 'b', '_' };
const string COLOR_STRINGS[] = { "Red", "Yellow", "Green", "Blue", "None" };

const char VALUE_CHARS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'D', 'R', 'S', 'W', 'X' };
const string VALUE_STRINGS[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Draw2", "Reverse", "Skip", "Wild", "Draw4 Wild" };

// The total number of cards in a deck, including duplicates
const int TOTAL_CARDS = 108;

const int N_COLORS = 4;
const int NO_COLOR_INDEX = 4;

// Indices of each range in the value array
const int N_VALUES = 15;
const int FIRST_NUMBER_INDEX = 0;
const int LAST_NUMBER_INDEX = 9;
const int FIRST_ACTION_INDEX = 10;
const int LAST_ACTION_INDEX = 12;
const int FIRST_WILD_INDEX = 13;
const int LAST_WILD_INDEX = 14;

// Indices of specific action and wild cards
const int DRAW2_INDEX = 10;
const int REVERSE_INDEX = 11;
const int SKIP_INDEX = 12;
const int WILD_INDEX = 13;
const int DRAW4_WILD_INDEX = 14;

// How many duplicates of each card type are in the deck
const int N_0_CARDS = 1;
const int N_NUMBER_CARDS = 2;
const int N_ACTION_CARDS = 2;
const int N_WILD_CARDS = 4;

const int ACTION_SCORE = 20;
const int WILD_SCORE = 50;

// An Uno card with a color and value (e.g. a number or action)
class Card
{
    public:
        Card();
        Card( int, int );

        int getColor() const;
        int getValue() const;
        string getColorAsString() const;
        string getValueAsString() const;
        string toStringShort() const;
        string toStringLong() const;
        void printShort() const;
        void printLong() const;

        int getScore() const;
        bool isAction() const;
        bool isWild() const;
        bool canPlayOn( Card, int ) const;

        bool isEqual( Card ) const;
        bool isLessThan( Card ) const;
        bool isGreaterThan( Card ) const;
    private:
        int color;
        int value;
};

#endif