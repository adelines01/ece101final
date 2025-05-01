#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CARDS 100
#define START_DECKSIZE 7 
#define MAX_PLAYERS 14 
#define MAX_SHUFFLE 10000
#define MAX_NAME 20

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
    int i, j, k, numPlayers, cardIndex;
    struct card;
    struct player;
    card gameDeck[MAX_CARDS];
    
    initializeDeck(gameDeck, MAX_CARDS);
    shuffleDeck(gameDeck);
    
    printf("Enter number of players: \n");
    scanf("%d", &numPlayers);
    
    player *playerInfo = (player*)malloc(numPlayers * sizeof(player));
    
    // FIXME: potentially shove this all in a function? like initializePlayers();
    // should initialize every player from one to numPlayers
    for (i = 0; i < numPlayers; i++) {
        printf("Enter Player %d's name: \n", (i + 1));
        
        // FIXME: only reads the second input "player one" as the second player, reads the num of players correctly, also extra newline?
        fgets(playerInfo[i].playerName, MAX_NAME, stdin);
        
        playerInfo[i].deck = (card*)malloc(START_DECKSIZE * sizeof(card));
        playerInfo[i].decksize = 0;
    }
    
    // FIXME: doesn't access the deck itself? so all hands are empty
    for (j = 0; j < START_DECKSIZE; j++) {
        for (k = 0; k < numPlayers; k++) {
            if (cardIndex < 100) {
                playerInfo[k].deck[j];
                playerInfo[k].decksize++;
                cardIndex;
            }
        }
    }
    
    for (i = 0; i < numPlayers; i++) {
        printPlayerHand(playerInfo[i]);
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
void printPlayerHand(player p) {
    int i;
    
    printf("%s's hand:\n", p.playerName);
    
    if (p.decksize == 0) {
        printf("Their hand is empty!\n");
        return;
    }
    
    for (i = 0; i < p.decksize; i++) {
        char cardName = p.deck[i].name;
        char cardColor = p.deck[i].color;
        
        if (cardName >= '0' && cardName <= '9') {
            char *colorStr = "";
            switch(cardColor) {
                case 'R': 
                    colorStr = "Red";
                    break;
                case 'Y':
                    colorStr = "Yellow";
                    break;
                case 'G':
                    colorStr = "Green";
                    break;
                case 'B':
                    colorStr = "Blue";
                    break;
                default:
                    colorStr = "Error - card color unknown";
                    break;
            }
            
            printf("%s %c\n", colorStr, cardName);
            
        }
        
        else {
            char *nameStr = "";
            switch(cardName) {
                case 'A':
                    nameStr = "AND";
                    break;
                case 'O':
                    nameStr = "OR";
                    break;
                case 'N':
                    nameStr = "NOT";
                    break;
                case 'R':
                    nameStr = "Reverse";
                    break;
                default:
                    nameStr = "Error - special card type unknown";
                    break;
            }
            
            printf("%s\n", nameStr);
        }
    }
    
    printf("\n");
}