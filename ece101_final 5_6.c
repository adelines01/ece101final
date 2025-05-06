#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CARDS 100
#define START_DECKSIZE 7 
#define MAX_PLAYERS 14 
#define MIN_PLAYERS 2
#define MAX_SHUFFLE 10000

typedef struct card_s {
    char name; // ’0’-‘9’ for number cards, ‘A’ for AND, ‘O’ for OR, ‘N’ for NOT, and ‘R’ for Reverse.
    char color; // ’R’ for red, ‘Y’ for yellow, ‘G’ for green, ‘B’ for blue, and ‘S’ for special cards
    struct card_s *listp;
} card;

// Each player has a name, deck, and the number of cards in their hands. (made deck a pointer to cards because the player might draw or lose cards)
typedef struct player_t {
    char playerName[20];
    card *deck; // Dynamic array of cards
    int decksize; // how many cards the player currently holds
} player;

// structure for the linked list (pile the players place cards into)
typedef struct played_pile_s {
    card *top;
    int total;
} played_pile;

void CreateDeck(card *deck, int deckSize);
void shuffleDeck(card *deck);
void initializeDeck(card *deck, int deckSize);
played_pile *createPile();

void printOrdinal(int numOfPlayer);
void printCard(card c);
void printPlayerHand(player *p);

int isValidCard(played_pile* pile, card candidate);
int drawCard(card *deck, int deckSize, player p);

// functions for testing the linked lists (probably able to keep them for the final code)
void addCard(played_pile *pile, char name, char color);
void printTopCard(played_pile *pile);
void freePile(played_pile *pile);


// test functions - - - - - - - - - -

    /** test to make sure names were printing (can eventually delete)
    for (i = 0; i < numPlayers; i++) {
        printf("%d %s\n", i + 1, players[i].playerName);
    }
    **/
    
    /** test that prints out the first 10 cards, to make sure the deck/shuffle is working correctly
    for (i = 0; i < 10; i++) {
        printf("Card %d: %c and %c\n", (i + 1), gameDeck[i].name, gameDeck[i].color);
    }
   **/
   
   /** tests adding a card to the pile and printing the new top card
    addCard(pile, '5', 'R'); // Red 5
    printTopCard(pile); // Should print "Top of card pile is: Red 5"
   **/
   
    /** Print player's hand
    for (i = 0; i < numPlayers; i++) {
        printPlayerHand(&players[i]);
    }
    **/

// main function - - - - - - - - - -
int main() {

    srand(time(0)); // Use the current time as the seed to ensure the program will get a unique sequence

    int i, j, k, numPlayers, cardChoice;
    int w = 0;
    int cardIndex = 0;
    card gameDeck[MAX_CARDS];
    
    initializeDeck(gameDeck, MAX_CARDS);
    shuffleDeck(gameDeck);
    
    // Get the number of players
    printf("Enter number of players: ");
    scanf("%d", &numPlayers);
    getchar();   // discard left over characters 
    
    if (numPlayers >= MIN_PLAYERS && numPlayers <= MAX_PLAYERS) {
        printf("\n");
    }
    
    // Array of players to store multiple player information
    player players[numPlayers];
    
    while (numPlayers < MIN_PLAYERS || numPlayers > MAX_PLAYERS) {
        printf("\n");
        printf("Invalid! Please enter a number of players between %d and %d\n", MIN_PLAYERS, MAX_PLAYERS);
        scanf("%d", &numPlayers);
        getchar();   // discard left over characters 
        printf("\n");
    }
    
    // Get player names 
    for (i = 0; i < numPlayers; i++) {
        printf("Enter ");
        printOrdinal(i + 1); // Print (first, second...)
        printf(" player’s name: \n");
        fgets(players[i].playerName, sizeof(players[i].playerName), stdin);

        // Trim newline from player name: use strcspn to replace newline charcter with null terminator 
        players[i].playerName[strcspn(players[i].playerName, "\n")] = '\0';
        
        // Dynamically allocate memory for player's hand
        players[i].deck = malloc(START_DECKSIZE * sizeof(card));
	    if (players[i].deck == NULL) {
	        printf("Failed to allocate memory");
		    return -1;  // Exit if memory could not be allocated
	    }
		    
        players[i].decksize = START_DECKSIZE;  // Set initial hand size of 7 
        
        for (j = 0; j < START_DECKSIZE; j++) {
            players[i].deck[j] = gameDeck[cardIndex++];  // Assign card from shuffled deck, cardIndex is used to move through the cards.
        }
    
    }
    
    // creates the pile for players to put cards on
    played_pile *pile = createPile();
    
    // loop for gameplay - eventually need to make this encompass the whole thing 
    // since i think that every game you'd have to reenter # of players/names/reshuffle deck?
    while (w == 0) {
        for (j = 0; j < numPlayers; j++) {
            // prints out each player's hand and the top card of the pile each round
            for (k = 0; k < numPlayers; k++) {
                printPlayerHand(&players[k]);
            }
            printTopCard(pile);
            
            printf("\n");
            printf("%s, enter which card to play from 0 to %d: ", players[j].playerName, players[j].decksize);
            scanf("%d", &cardChoice);
            
            // runs while the current player doesn't pick a card currently in their hand
            while (cardChoice > players[j].decksize) {
                printf("Invalid choice, %s does not have %d cards.\n", players[j].playerName, cardChoice);
                printf("%s, enter which card to play from 0 to %d: \n", players[j].playerName, players[j].decksize);
                scanf("%d", &cardChoice);
            }
            
            // FIXME: looped branch to check if all of player's cards are valid; if not, forces them to draw and skips entry
            
            // FIXME: here would go the main thing - playing the card, discarding it from the player's hand, moves it to the top of the pile, etc.
            card playedCard = players[j].deck[cardChoice];
            
            // branch if the current player has 0 cards - winner!
            if (players[j].decksize == 0) {
                printf("%s wins!\n", players[j].playerName);
                w = 1;
                break; 
            }
            
            // FIXME: branch where it checks if the pile players draw from is empty, and then if so, who has the least cards
            
        }
        
        // if it's at the last player and there's no winner, resets the counter 
        if (j == (numPlayers - 1)) {
                j = 0;
        }
    }
    
    // frees the memory of the pile
    freePile(pile);
    
    // Free the allocated memory for each player
    for (i = 0; i < numPlayers; i++) {
        free(players[i].deck);
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

    for (int i = 0; i < MAX_SHUFFLE; i++) {
        int first = rand() % MAX_CARDS;  // Generate a random number 0-99
        int second = rand() % MAX_CARDS;

        // swap the indexes
        card temp = deck[first];  // temp is of type card to hold the entire card being swapped
        deck[first] = deck[second];
        deck[second] = temp;
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
            printf("AND");
        }
        else if (c.name == 'O') {
            printf("OR");
        }
        else if (c.name == 'N') {
            printf("NOT");
        }
        else if (c.name == 'R') {
            printf("Reverse");
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
void printPlayerHand(player *p) {
    int i;
    
    printf("\n");
    printf("%s's hand: \n", p->playerName);
    printf("\n");

    for (i = 0; i < p->decksize; i++) {
        printCard(p->deck[i]);
        printf("\n"); // New line after each card 
    }
}
    
// Print the ordinal word for players (first, second...)
void printOrdinal(int numOfPlayer) {
    if (numOfPlayer == 1) {
        printf("first");
    } else if (numOfPlayer == 2) {
        printf("second");
    } else if (numOfPlayer == 3) {
        printf("third");
    } else if (numOfPlayer == 4) {
        printf("fourth");
    } else if (numOfPlayer == 5) {
        printf("fifth");
    } else if (numOfPlayer == 6) {
        printf("sixth");
    } else if (numOfPlayer == 7) {
        printf("seventh");
    } else if (numOfPlayer == 8) {
        printf("eighth");
    } else if (numOfPlayer == 9) {
        printf("ninth");
    } else if (numOfPlayer == 10) {
        printf("tenth");
    } else if (numOfPlayer == 11) {
        printf("eleventh");
    } else if (numOfPlayer == 12) {
        printf("twelfth");
    } else if (numOfPlayer == 13) {
        printf("thirteenth");
    } else if (numOfPlayer == 14) {
        printf("fourteenth");
    }
}

// Draw a single card from the deck and place it in a player's hand. Return 1 if the draw was successful, 0 if deck is empty.
// draws from the END of the deck and then reduces the deck size to keep things simpler
int drawCard(card *deck, int *deckSize, player *p) {
    int success = 1;
    int empty = 0;
    
    if (*deckSize == 0) {
        return empty;
    }
    
    card draw = deck[*deckSize - 1];
    
    p->deck = realloc(p->deck, (p->decksize + 1) * sizeof(card));
    if (p->deck == NULL) {
        return empty;
    }
    
    p->deck[p->decksize] = drawn;
    p->decksize++;
    (*decksize)--;
    
    return success;
}

// Check if a candidate card is valid to play on the top-of-pile card. Return 1 if valid, 0 otherwise.
// note: changed the parameters from what's on the sheet, this is said to be allowed as long as the function name itself is the same so we should be okay
// FIXME/extra note: need to make it so we can actually input which card to add to test it i think. oopsies
int isValidCard(played_pile *pile, card candidate) {
    card pileTopCard;
    int valid = 1;
    int not_valid = 0;
    
    pileTopCard.name = pile->top->name;
    pileTopCard.color = pile->top->color;
    
    // if there's nothing in the pile, anything can be placed
    if (pile->top == NULL) {
        return valid;
    }
    // if the colors or numbers match, or a special card is being placed, then these are valid
    else if ( (candidate.color == pileTopCard.color) || (candidate.name == pileTopCard.name) || (candidate.color == 'S') ) {
        return valid;
    }
    // any other option/combination
    else {
        return not_valid;
    }
}

// test functions for the pile: creating and erasing the pile, along with adding and printing the top card (NOT FINAL)
// again, feel free to mess with these, change the parameters, etc. if anything stops working or doesn't work as it should

// creates the pile, making sure everything is set to null/0 so that it starts "empty"
played_pile *createPile() {
    played_pile *pile = (played_pile*)malloc(sizeof(played_pile));
    
    if (pile == NULL) {
        printf("Failed to allocate memory for the pile on which cards are played.\n");
        return NULL;
    }
    
    pile->top = NULL;
    pile->total = 0;
    
    return pile;
}

// initializes and adds a new card and its values to the top of the pile
void addCard(played_pile *pile, char name, char color) {
    card *newCard = (card*)malloc(sizeof(card));
    if (newCard == NULL) {
        printf("Allocation of memory failed for the new card.\n");
        return;
    }
    
    newCard->name = name;
    newCard->color = color;
    newCard->listp = pile->top;
    pile->top = newCard;
    pile->total++;
}

// prints the top card by assigning "newCard" with the values the pile->top point to
void printTopCard(played_pile *pile) {
    card newCard;
    
    if (pile->top == NULL) {
        printf("\n");
        printf("Card pile is empty.\n");
        return;
    }
    
    newCard.name = pile->top->name;
    newCard.color = pile->top->color;
    
    printf("\n");
    printf("Top of card pile is: ");
    printCard(newCard);
    printf("\n");
}

// frees the memory of each card in the pile, then the pile itself
void freePile(played_pile *pile) {
    card *current = pile->top;
    while (current != NULL) {
        card *temp = current;
        current = current->listp;
        free(temp);
    }
    free(pile);
}