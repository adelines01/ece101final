#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CARDS 100
#define START_DECKSIZE 7 
#define MAX_PLAYERS 8 
#define MIN_PLAYERS 2
#define MAX_SHUFFLE 10000
#define DRAW_PENALTY 4

typedef struct card_t {
    char name; // ’0’-‘9’ for number cards, ‘A’ for AND, ‘O’ for OR, ‘N’ for NOT, and ‘R’ for Reverse.
    char color; // ’R’ for red, ‘Y’ for yellow, ‘G’ for green, ‘B’ for blue, and ‘S’ for special cards
    struct card_t *listp;
} card;

// Each player has a name, deck, and the number of cards in their hands. (made deck a pointer to cards because the player might draw or lose cards)
typedef struct player_t {
    char playerName[20];
    card *deck; // Dynamic array of cards
    int decksize; // how many cards the player currently holds
} player;

// TESTING: structure for the linked list (pile the players place cards into)
typedef struct played_pile_s {
    card *top;
    int total;
} played_pile;

void CreateDeck(card deck[], int deckSize);
void shuffleDeck(card *deck);
void initializeDeck(card *deck, int deckSize);
played_pile *createPile();

void printNumLabel(int numOfPlayer);
void printCard(card c);
void printPlayerHand(player *p);

int isValidCard(played_pile *pile, card candidate);

// functions for testing the linked lists (probably able to keep them for the final code)
void addCard(played_pile *pile, char name, char color);
void printTopCard(played_pile *pile);
void freePile(played_pile *pile);

// // Handle effect of AND card. This could check the next player’s hand for the correct color AND number (return 1), or apply a penalty if they cannot match (return 0)
//int handleAND(player *p, player *nextPlayer, card topOfPile, card *gameDeck, int *deckIndex, played_pile *pile);



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

// main function - - - - - - - - - -
int main() {
    srand(time(0)); // get a random seed based on the current time so we get different results each run

    int i, j, k, numPlayers, cardChoice;
    char playAgain = 'y';   // initialize with y so we play at least once
    
    // Keep asking if the user wants to play again until they say 'no'
    while (playAgain != 'n' && playAgain != 'N') {
        int gameIndex = 0;
        card gameDeck[MAX_CARDS];  // will hold the deck of cards
        initializeDeck(gameDeck, MAX_CARDS);  // create a fresh deck
        shuffleDeck(gameDeck);  // shuffle the deck
        int firstCard;    // Store player one's first card index
        
        // Get the number of players
        printf("Enter number of players: \n");
        scanf("%d", &numPlayers);
        getchar();   // gotta clear that newline character so the next input stops getting skipped
        
        if (numPlayers >= MIN_PLAYERS && numPlayers <= MAX_PLAYERS){
            printf("\n");
        }
        
        // An array to hold each player's info
        player players[MAX_PLAYERS];
        
        // Number of players needs to be between 0 and 14
        while (numPlayers < MIN_PLAYERS || numPlayers > MAX_PLAYERS) {
            printf("\n");
            printf("Invalid! Please enter a number of players between %d and %d\n", MIN_PLAYERS, MAX_PLAYERS);
            printf("Enter number of players: \n");
            scanf("%d", &numPlayers);
            getchar();  // Make sure the next input isn't skipped
            printf("\n");
        }
        
        // get each players name and deal them 7 cards
        for (i = 0; i < numPlayers; i++) {
            printf("Enter ");
            printNumLabel(i + 1); // Print "first, second, etc" for each player
            printf(" player’s name: \n");
            fgets(players[i].playerName, sizeof(players[i].playerName), stdin);
    
            // cuts off extra space at the end of the player name
            players[i].playerName[strcspn(players[i].playerName, "\n")] = '\0';
            
            // Dynamically allocate memory for player's hand
            players[i].deck = (card *)malloc(sizeof(card) * START_DECKSIZE);

    	    if (players[i].deck == NULL) {
    	        printf("Failed to allocate memory");
    		    return -1;  // Exit if memory could not be allocated
    	    }
    		    
            players[i].decksize = START_DECKSIZE;  // Start with a hand of 7 cards
            
            // Assign 7 cards to each player
            for (int j = 0; j < START_DECKSIZE; j++) {
                players[i].deck[j] = gameDeck[gameIndex++];  // keeps track of where we are in the main deck
            }
        
        }
        
        // Print everyones starting hand
        for (i = 0; i < numPlayers; i++) {
            printPlayerHand(&players[i]);
        }
        
        printf("\nCard pile is empty\n");
        
        // creates the pile for players to put cards on
        played_pile *pile = createPile();
        int firstCardCheck = 0;   // will check if player one has played their first card
        
        // get the first player to put down a valid card
        while (firstCardCheck == 0) {
            printf("%s, enter which card to play from 0 to 6: \n", players[0].playerName);
            scanf("%d", &firstCard);
            getchar();
                    
            if (firstCard < 0 || firstCard >= players[0].decksize){
                printf("Invalid choice, %s does not have %d cards\n", players[0].playerName, firstCard + 1);
            }
            else if (players[0].deck[firstCard].name =='A' || players[0].deck[firstCard].name =='O' || players[0].deck[firstCard].name =='N' || players[0].deck[firstCard].name =='R') {
                printf("Invalid choice, first pile card cannot be a special card\n");
            }
            else {
                // put the first card on the pile
                card played = players[0].deck[firstCard];
                addCard(pile, played.name, played.color);
            
                // Remove the card from the player's hand
                for (i = firstCard; i < players[0].decksize - 1; i++) {
                    players[0].deck[i] = players[0].deck[i + 1];
                }
                players[0].decksize--;  // can't forget to update the player's deck size
                    
                firstCardCheck = 1;  // card looks good
            }
        }
        
        int w = 0;
        while (w == 0) {
            for (j = 0; j < numPlayers; j++) {
                
                if (firstCardCheck == 1) {
                    j++;
                    firstCardCheck = 2;
                }
                
                // prints out each player's hand and the top card of the pile each round
                for (k = 0; k < numPlayers; k++) {
                    printPlayerHand(&players[k]);
                }
                printTopCard(pile);
                
                printf("\n");
                printf("%s, enter which card to play from 0 to %d: ", players[j].playerName, players[j].decksize - 1);
                scanf("%d", &cardChoice);
                
                // runs while the current player doesn't pick a card currently in their hand
                while (cardChoice > players[j].decksize) {
                    printf("\n");
                    printf("Invalid choice, %s does not have %d cards.\n", players[j].playerName, cardChoice);
                    printf("%s, enter which card to play from 0 to %d: \n", players[j].playerName, players[j].decksize);
                    scanf("%d", &cardChoice);
                }
                
                // FIXME: looped branch to check if all of player's cards are valid; if not, forces them to draw and skips entry
                
                // main loop
                card playedCard = players[j].deck[cardChoice];
                
                while ( isValidCard(pile, playedCard) != 1) {
                    card topCard;
                    
                    topCard.name = pile->top->name;
                    topCard.color = pile->top->color;
                    
                    printf("\n");
                    printf("Invalid choice, cannot play ");
                    printCard(playedCard);
                    printf(" on ");
                    printCard(topCard);
                    printf("\n");
                    
                    printf("\n");
                    printf("%s, enter which card to play from 0 to %d: \n", players[j].playerName, players[j].decksize);
                    scanf("%d", &cardChoice);
                    
                    card playedCard = players[j].deck[cardChoice];
                }
                
                addCard(pile, playedCard.name, playedCard.color);
                
                //Remove the card from the player's hand
                for (k = cardChoice; k < players[j].decksize - 1; k++) {
                    players[j].deck[k] = players[j].deck[k + 1];
                }
                players[j].decksize--;  // can't forget to update the player's deck size
                
                // branch if the current player has 0 cards - winner!
                if (players[j].decksize < 0) {
                    printf("%s wins!\n", players[j].playerName);
                    w = 1;
                    break; 
                }
                
                /** // FIXME: branch where it checks if the pile players draw from is empty, and then if so, who has the least cards
                if (deckSize == 0) {
                    char winner[20];
                    for (k = 0; k < numPlayers; k++) {
                        if (players[k].decksize > players[k + 1].decksize) {
                            strcpy(winner, players[k].playerName);
                        }
                    }
                    
                    printf("%s wins!\n", winner);
                    w = 1;
                    break;
                } **/
                
            }
        
            // if it's at the last player and there's no winner, resets the counter 
            if (j == (numPlayers - 1)) {
                j = 0;
            }
        }
    
        // !! - - - placeholder: where the main game (loop?) should probably go - - - !!
        /*while (){
            mmmm
        
        
        // adds and prints the card at the top of the pile
        addCard(pile, '5', 'R'); // Red 5
        printTopCard(pile); // should print "Top of card pile is: Red 5"
        
        }*/
        
        // frees the memory of the pile
        freePile(pile);
        
        // Free the allocated memory for each player
        for (int i = 0; i < numPlayers; i++) {
            free(players[i].deck);
        }  
        
        // Print game over message
        // - printf("\nPlayer Two wins!\n") -- want to print the player name that wins
        printf("\nPlay again? (y/n)?: \n");
        scanf(" %c", &playAgain);
        getchar(); // don't want the next input to be skipped
        printf("\n");
    }

    printf("Goodbye\n");
    
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

// Shuffle a deck of 100 cards by swapping the position of two cards
void shuffleDeck(card *deck) {

    // repeat swap 10000 times for proper shuffling 
    for (int i = 0; i < MAX_SHUFFLE; i++) {
        
        // get two random number 0-99 for the index's
        int first = rand() % MAX_CARDS;  
        int second = rand() % MAX_CARDS;
        
        card temp = deck[first];
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

    //
    for (i = 0; i < p->decksize; i++) {
        printCard(p->deck[i]);  // print each card in a player's hand
        printf("\n");
    }
}
    
// Converts number like 1 to first
void printNumLabel(int numOfPlayer) {
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

// Check if a candidate card is valid to play on the top-of-pile card. Return 1 if valid, 0 otherwise.
// note: changed the parameters from what's on the sheet, this is said to be allowed as long as the function name itself is the same so we should be okay
// extra note: need to make it so we can actually input which card to add to test it i think. oopsies
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
    else if ( (candidate.color == pileTopCard.color) || (candidate.name == pileTopCard.name) || (candidate.color == 'S' || pileTopCard.color == 'S') ) {
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

// Handle effect of AND card. This could check the next player’s hand for the correct color AND number (return 1), or apply draw 4 penalty if they cannot match (return 0)
/*int handleAND(player *p, player *nextPlayer, card topOfPile, card *gameDeck, int *deckIndex, played_pile *pile){
    
    // check the player's hand for a matching card 
    for (int i = 0; i < nextPlayer->decksize; i++) {
	   if (nextPlayer->deck[i].color == topOfPile.color && nextPlayer->deck[i].name == topOfPile.name) {
	            
        printf("Card Matches, no AND penalty\n");

        // hold on to the card that will be added to pile
        card pileCard = nextPlayer->deck[i];

        // take out the card from the players hand & move the rest over to fill open space
        for (int j = i; j < nextPlayer->decksize - 1; j++) {
            nextPlayer->deck[j] = nextPlayer->deck[j + 1];
        }
        nextPlayer->decksize--; // Decrease deck size since player has one less card

        // Add the card to the played pile
        addCard(pile, pileCard.name, pileCard.color);

        return 1;  // match found
        }
    }
	
    // No match: player draws 4
    printf("%s has no card that matches %s and %d\n", nextPlayer->playerName, topOfPile.color, topOfPile.name);
    printf("\n");
    printf("AND penalty, Draw 4\n");

    // add 4 cards from deck to player's hand
    for (int i = 0; i < DRAW_PENALTY; i++) {
        if (*deckIndex < MAX_CARDS) {
        
            // See if we need a bigger hand size for the player
            card *newDeck = (card *)realloc(nextPlayer->deck, (nextPlayer->decksize + 1) * sizeof(card));
            if (newDeck == NULL) {
                printf("Failed to allocate memory while drawing cards.\n");
                return -1;
            }
            nextPlayer->deck = newDeck;
            nextPlayer->deck[nextPlayer->decksize] = gameDeck[*deckIndex]; // grab a card from the main deck and add it to player's hand
            nextPlayer->decksize++; // player has one more card now
            (*deckIndex)++; // Move to the next card in the main deck
        }
    }
	
	    return 0;  // Return 0 if no match is found, and the player has drawn 4 cards
}*/
