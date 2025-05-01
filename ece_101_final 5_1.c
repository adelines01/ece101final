#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CARDS 100
#define START_DECKSIZE 7 
#define MAX_PLAYERS 14 
#define MAX_SHUFFLE 10000

typedef struct card_s {
    char name; // ’0’-‘9’ for number cards, ‘A’ for AND, ‘O’ for OR, ‘N’ for NOT, and ‘R’ for Reverse.
    char color; // ’R’ for red, ‘Y’ for yellow, ‘G’ for green, ‘B’ for blue, and ‘S’ for special cards
} card;

// Each player has a name, deck, and the number of cards in their hands. (made deck a pointer to cards because the player might draw or lose cards)
typedef struct player_t {
    char playerName[20];
    card *deck; // Dynamic array of cards
    int decksize; // how many cards the player currently holds
} player;

void CreateDeck(card deck[], int deckSize);

// Shuffle deck
void shuffleDeck(card *deck);

void initializeDeck(card *deck, int deckSize);

void printCard(card c);
void printPlayerHand(player p);


// main function - - - - - - - - - -
int main() {
    int i, j, numPlayers;
    struct card;
    struct player;
    card gameDeck[MAX_CARDS];
    
    initializeDeck(gameDeck, MAX_CARDS);
    shuffleDeck(gameDeck);
    
    // test function that prints out the first 10 cards, to make sure the deck/shuffle is working correctly
    for (i = 0; i < 10; i++) {
        printf("Card %d: %c and %c\n", (i + 1), gameDeck[i].name, gameDeck[i].color);
    }
    
    return 0;
}

// Your code must have a CreateDeck() function that creates a standard deck of 100 cards, that could be used for any card playing game.
void CreateDeck(card *deck, int deckSize) {
    int i = 0;
    int j, color, set, special;
    char number;
    char color_cards[] = {'R', 'Y', 'G', 'B'};
    char special_cards[] = {'A', 'O', 'N', 'R'};
    
    for (color = 0; color < 4; color++) {
        for (set = 0; set < 2; set++) {
            for (number = '0'; number <= '9'; number++) {
                deck[i].name = number;
                deck[i].color = color_cards[color];
                i++;
            }
        }
    }
    
    for (special = 0; special < 4; special++) {
        for (j = 0; j < 5; j++) {
            deck[i].name = special_cards[special];
            deck[i].color = 'S';
            i++;
        }
    }
}

// Shuffle a deck of 100 cards
void shuffleDeck(card *deck) {
    srand(time(0)); // Use the current time as the seed to ensure the program will get a unique sequence
    
    for (int j = 0; j < MAX_SHUFFLE; j++) {
        for (int i = 0; i < MAX_CARDS; i++) {
            int swapIndex = rand() % MAX_CARDS;
            card temp = deck[i];  // temp is of type card to hold the entire card being swapped
            deck[i] = deck[swapIndex];
            deck[swapIndex] = temp;
        }
    }
    
}

// Creates a standard deck of 100 cards (two sets of 0-9 for each color, plus the special cards)
void initializeDeck(card *deck, int deckSize) {
    int i = 0;
    int j, color, set, special;
    char number;
    char color_cards[] = {'R', 'Y', 'G', 'B'};
    char special_cards[] = {'A', 'O', 'N', 'R'};
    
    for (color = 0; color < 4; color++) {
        for (set = 0; set < 2; set++) {
            for (number = '0'; number <= '9'; number++) {
                deck[i].name = number;
                deck[i].color = color_cards[color];
                i++;
            }
        }
    }
    
    for (special = 0; special < 4; special++) {
        for (j = 0; j < 5; j++) {
            deck[i].name = special_cards[special];
            deck[i].color = 'S';
            i++;
        }
    }
}

// Print a single card (e.g., "Red 5", "Blue 0", "AND", "Reverse", etc.).
void printCard(card c) {
    
    if (c.color == 'S') {
        if (c.name == 'A') {
            printf("AND\n");
        }
        else if (c.name == 'O') {
            printf("OR\n");
        }
        else if (c.name == 'N') {
            printf("NOT\n");
        }
        else if (c.name == 'R') {
            printf("Reverse\n");
        }
        // FIXME: possibly remove before turning in? i used it to test the function, but the insurance might be nice
        else {
            printf("Error: Card is special but not equal to A, O, N, or R.\n");
        }
    }
    
    else {
        if (c.color == 'R') {
            printf("Red %c", c.name);
        }
        else if (c.color == 'Y') {
            printf("Yellow %c", c.name);
        }
        else if (c.color == 'G') {
            printf("Green %c", c.name);
        }
        else if (c.color == 'B') {
            printf("Blue %c", c.name);
        }
        // FIXME: possibly remove before turning in? i used it to test the function, but the insurance might be nice
        else {
            printf("Error: Card isn't special but is not equal to R, Y, G, or B.\n");
        }
    }
}

// Print the hand of a given player (their name + the cards).
// FIXME: this doesnt work :(
/** void printPlayerHand(player p) {
    int i;
    card c;
    
    printf("%s has the cards: \n", player.playerName);
    
    for (i = 0; i < player.decksize; i++) {
        printCard(c[i]); printf(" ,");
    }
} **/