#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DECK 100
#define START_DECKSIZE 7 
#define MAX_PLAYERS 8 
#define MIN_PLAYERS 2
#define MAX_SHUFFLE 10000
#define DRAW_PENALTY 4
#define NORMAL_ORDER 1
#define REVERSE_ORDER -1

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
int drawCard(card *deck, int *deckSize, player *p);

// functions for testing the linked lists (probably able to keep them for the final code)
void addCard(played_pile *pile, char name, char color);
void printTopCard(played_pile *pile);
void freePile(played_pile *pile);

int handleAND(player *currentPlayer, player *nextPlayer, card *gameDeck, int *deckSize, played_pile *pile);
int handleOR(player *currentPlayer, player *nextPlayer, card *gameDeck, int *deckSize, played_pile *pile);
int handleNOT(int currentPlayerIndex, int playerCount);
void handleReverse(int *reverseFlag, int *currentPlayerIndex, int playerCount);
int applyPenalty(player *p, card *deck, int *deckSize, int count);

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

    int i, j, k, numPlayers, cardChoice, newCardChoice;
    int *jPtr = &j;
    int ValidCount = 0;
    char playAgain = 'y';   // initialize with y so we play at least once
    
    // Keep asking if the user wants to play again until they say 'no'
    while (playAgain != 'n' && playAgain != 'N') {
        int gameIndex = 0;
        int gameDeckSize = 100;
        int *gameDeckSizePtr = &gameDeckSize;
        int turnOrder = 1;
        int *turnOrderPtr = &turnOrder;
        int currentPlayer = 0;
        card gameDeck[100];  // will hold the deck of cards
        initializeDeck(gameDeck, 100);  // create a fresh deck
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
                
            if (firstCardCheck == 1) {
                currentPlayer++;
                firstCardCheck = 2;
            }
                
            // resets validity increments
            i = 0;
            ValidCount = 0;
                
            // prints out each player's hand and the top card of the pile each round
            for (k = 0; k < numPlayers; k++) {
                printPlayerHand(&players[k]);
            }
            printTopCard(pile);
                
            // looped branch to check if all of current player's cards are valid
            for (i = 0; i < players[currentPlayer].decksize; i++) {
                card userCard = players[currentPlayer].deck[i];
                    
                if ( isValidCard(pile, userCard) == 1 ) {
                    ValidCount++;
                }
            }
                
            // if the user has no cards that match, forces them to draw and skips entry
            if (ValidCount == 0) {
                if ( drawCard(gameDeck, gameDeckSizePtr, &players[currentPlayer]) == 1 ) {
                    printf("%s has no card that matches %c or %c, draw one and skip turn.\n", players[currentPlayer].playerName, pile->top->color, pile->top->name);
                    if (currentPlayer < (numPlayers - 1) ) {
                        continue;
                    }
                    else {
                        continue;
                    }
                }
                else {
                    printf("Error with draw deck.\n");
                }
            }
                
            printf("\n");
            printf("%s, enter which card to play from 0 to %d: ", players[currentPlayer].playerName, players[currentPlayer].decksize - 1);
            scanf("%d", &cardChoice);
            getchar();
                
            // runs while the current player doesn't pick a card currently in their hand
            while (cardChoice > players[currentPlayer].decksize) {
                printf("\n");
                printf("Invalid choice, %s does not have %d cards.\n", players[currentPlayer].playerName, cardChoice);
                printf("%s, enter which card to play from 0 to %d: ", players[currentPlayer].playerName, (players[currentPlayer].decksize - 1));
                scanf("%d", &cardChoice);
                getchar();
                printf("\n");
            }
                
            // main loop
            card playedCard = players[currentPlayer].deck[cardChoice];
                
            card topCard;
                    
            topCard.name = pile->top->name;
            topCard.color = pile->top->color;
            
            while (topCard.color == 'S' && playedCard.color == 'S') {
                printf("\n");
                printf("Invalid choice, cannot play two special cards in a row.\n");
                printf("%s, enter which card to play from 0 to %d: ", players[currentPlayer].playerName, (players[currentPlayer].decksize - 1));
                scanf("%d", &cardChoice);
                getchar();
                printf("\n");
                playedCard = players[currentPlayer].deck[cardChoice];
            }
                
            // runs if a special card is played
            if (playedCard.color == 'S') {
                if (playedCard.name == 'A') {
                    int andValid = 2;
                        
                    for (k = cardChoice; k < players[currentPlayer].decksize - 1; k++) {
                        players[currentPlayer].deck[k] = players[currentPlayer].deck[k + 1];
                    }
                    players[j].decksize--;  // can't forget to update the player's deck size
                        
                    if (currentPlayer >= (numPlayers - 2)) {
                        andValid = handleAND(&players[currentPlayer], &players[0], gameDeck, gameDeckSizePtr, pile);
                        currentPlayer = 0;
                    }
                    else {
                        andValid = handleAND(&players[currentPlayer], &players[currentPlayer + 1], gameDeck, gameDeckSizePtr, pile);
                        currentPlayer++;
                    }
                        
                    if (andValid == 0) {
                        printf("Penalty applied.\n");
                    }
                    else {
                        printf("No penalty applied!\n");
                    }
                }
                    
                if (playedCard.name == 'O') {
                    int orValid = 2;
                        
                    for (k = cardChoice; k < players[currentPlayer].decksize - 1; k++) {
                        players[currentPlayer].deck[k] = players[currentPlayer].deck[k + 1];
                    }
                    players[currentPlayer].decksize--;  // can't forget to update the player's deck size
                        
                    if (currentPlayer >= (numPlayers - 2)) {
                        orValid = handleOR(&players[currentPlayer], &players[0], gameDeck, gameDeckSizePtr, pile);
                        currentPlayer = 0;
                    }
                    else {
                        orValid = handleOR(&players[currentPlayer], &players[currentPlayer + 1], gameDeck, gameDeckSizePtr, pile);
                        currentPlayer++;
                    }
                        
                    if (orValid == 0) {
                        printf("Penalty applied.\n");
                    }
                    else {
                        printf("No penalty applied!\n");
                    }
                }
                    
                if (playedCard.name == 'N') {
                        
                    for (k = cardChoice; k < players[currentPlayer].decksize - 1; k++) {
                        players[currentPlayer].deck[k] = players[currentPlayer].deck[k + 1];
                    }
                    players[currentPlayer].decksize--;  // can't forget to update the player's deck size
                        
                    currentPlayer = handleNOT(currentPlayer, numPlayers);
                }
                    
                // FIXME: results in memory error
                if (playedCard.name == 'R') {
                    /** for (k = cardChoice; k < players[currentPlayer].decksize - 1; k++) {
                        players[currentPlayer].deck[k] = players[currentPlayer].deck[k + 1];
                    }
                    players[currentPlayer].decksize--;  // can't forget to update the player's deck size
                        
                    handleReverse(turnOrderPtr, jPtr, numPlayers);
                        
                    *turnOrderPtr *= -1;
                        
                    if (numPlayers == MIN_PLAYERS) {
                        currentPlayer = ( (currentPlayer + turnOrder + numPlayers) % numPlayers );
                    } **/
                    printf("Teehee\n");
                        
                }
                    
                continue;
            }
                
            while ( isValidCard(pile, playedCard) == 0 ) {
                printf("\n");
                printf("Invalid choice, cannot play ");
                printCard(playedCard);
                printf(" on ");
                printCard(topCard); 
                printf("\n");
                    
                printf("\n");
                printf("%s, enter which card to play from 0 to %d: \n", players[currentPlayer].playerName, (players[currentPlayer].decksize - 1));
                scanf("%d", &newCardChoice);
                getchar();
                printf("\n");

                playedCard = players[currentPlayer].deck[newCardChoice];
            }
                
            addCard(pile, playedCard.name, playedCard.color);
                
            // Remove the card from the player's hand
            if (playedCard.color != 'S') {
                for (k = cardChoice; k < players[currentPlayer].decksize - 1; k++) {
                    players[currentPlayer].deck[k] = players[currentPlayer].deck[k + 1];
                }
                players[currentPlayer].decksize--;  // can't forget to update the player's deck size 
            }
                
                
            // branch if the current player has 0 cards - winner!
            if ( (players[currentPlayer].decksize - 1) < 0) {
                printf("%s wins!\n", players[currentPlayer].playerName);
                w = 1;
                break; 
            }
                
            // FIXME: branch where it checks if the pile players draw from is empty, and then if so, who has the least cards
            if (gameDeckSizePtr == 0) {
                char winner[20];
                for (k = 0; k < (numPlayers - 1); k++) {
                    if (players[k].decksize > players[k + 1].decksize) {
                        strcpy(winner, players[k].playerName);
                    }
                }
                    
                printf("%s wins!\n", winner);
                w = 1;
                break;
            }
            
            if (currentPlayer >= (numPlayers - 1)) {
                currentPlayer = 0;
            }
            else {
                currentPlayer++;
            }
        }
        
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
        int first = rand() % 100;  
        int second = rand() % 100;
        
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
        printf("%d: ", i);
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
    }
}

// Check if a candidate card is valid to play on the top-of-pile card. Return 1 if valid, 0 otherwise.
// note: changed the parameters from what's on the sheet, this is said to be allowed as long as the function name itself is the same so we should be okay
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
    else if ( (candidate.color == 'S') || (pileTopCard.color == 'S') || (candidate.color == pileTopCard.color) || (candidate.name == pileTopCard.name) ) {
        return valid;
    }
    // any other option/combination
    else {
        return not_valid;
    }
}

// Draw a single card from the deck and place it in a player's hand. Return 1 if the draw was successful, 0 if deck is empty.
// draws from END of deck to make it easier to remove cards
int drawCard(card *deck, int *deckSize, player *p) {
    int success = 1;
    int empty = 0;
    
    if (*deckSize <= 0) {
        return empty;
    }
    
    card pickedCard = deck[*deckSize - 1];
    
    p->deck = realloc(p->deck, (p->decksize + 1) * sizeof(card));
    if (p->deck == NULL) {
        return empty;
    }
    
    p->deck[p->decksize] = pickedCard;
    p->decksize++;
    
    (*deckSize)--;
    
    return success;
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

// Handle effect of AND card. This could check the next player’s hand for the correct color AND number (return 1), 
// or apply a penalty if they cannot match (return 0)
int handleAND(player *currentPlayer, player *nextPlayer, card *gameDeck, int *deckSize, played_pile *pile) {
    
    // Let current player select a card from their hand
    printf("%s, choose a card to play for AND:\n", currentPlayer->playerName);
    printPlayerHand(currentPlayer);
    
    int cardIndex = -1;
    int w = 1;
    while (w = 1) {
        printf("Enter card choice from 0 to %d: ", currentPlayer->decksize - 1);
        scanf("%d", &cardIndex);
        // FIXME: exits the loop if another special card is played
        if (currentPlayer->deck[cardIndex].color == 'S') {
            printf("Invalid selection - cannot play a special card on another special card.\n");
        }
        else if (cardIndex >= 0 && cardIndex < currentPlayer->decksize) {
            w = 0;
            break;
        }
        else {
            printf("Invalid selection!\n");
        }
    }
    
    // Get selected card's properties
    char targetColor = currentPlayer->deck[cardIndex].color;
    char targetName = currentPlayer->deck[cardIndex].name;
    
    // Check next player's hand for a match
    int matchFound = 0;
    int foundCard = 0;
    
    for (int i = 0; i < nextPlayer->decksize; i++) {
        if (nextPlayer->deck[i].color == targetColor && 
            nextPlayer->deck[i].name == targetName) {
            matchFound = 1;
            int foundCard = i;
            
            break;
        }
    }
    
    if (matchFound) {
        printf("Card matches, no AND penalty.\n");
        
        int k = 0;
        for (int k = foundCard; k < nextPlayer->decksize - 1; k++) {
            nextPlayer->deck[k] = nextPlayer->deck[k + 1];
        }
        nextPlayer->decksize--;  // can't forget to update the player's deck size
        
        return 1;
    } else {
        printf("%s has no matching card, applying AND penalty.\n", nextPlayer->playerName);
        applyPenalty(nextPlayer, gameDeck, deckSize, 4); // Draw 4 cards
        return 0;
    }
}

int handleOR(player *currentPlayer, player *nextPlayer, card *gameDeck, int *deckSize, played_pile *pile) {
    
    // Let current player select a card from their hand
    printf("%s, choose a card to play for OR:\n", currentPlayer->playerName);
    printPlayerHand(currentPlayer);
    
    int cardIndex = -1;
    int w = 1;
    while (w = 1) {
        printf("Enter card choice from 0 to %d: ", currentPlayer->decksize - 1);
        scanf("%d", &cardIndex);
        // FIXME: exits the loop if another special card is played
        if (currentPlayer->deck[cardIndex].color == 'S') {
            printf("Invalid selection - cannot play a special card on another special card.\n");
        }
        else if (cardIndex >= 0 && cardIndex < currentPlayer->decksize) {
            w = 0;
            break;
        }
        else {
            printf("Invalid selection!\n");
        }
    }
    
    // Get selected card's properties
    char targetColor = currentPlayer->deck[cardIndex].color;
    char targetName = currentPlayer->deck[cardIndex].name;
    
    // Check next player's hand for a match
    int matchFound = 0;
    int foundCard = 0;
    
    for (int i = 0; i < nextPlayer->decksize; i++) {
        if (nextPlayer->deck[i].color == targetColor || 
            nextPlayer->deck[i].name == targetName) {
            matchFound = 1;
            int foundCard = i;
            
            break;
        }
    }
    
    if (matchFound) {
        printf("Card matches, no OR penalty.\n");
        
        int k = 0;
        for (int k = foundCard; k < nextPlayer->decksize - 1; k++) {
            nextPlayer->deck[k] = nextPlayer->deck[k + 1];
        }
        nextPlayer->decksize--;  // can't forget to update the player's deck size
        
        return 1;
    } else {
        printf("%s has no matching card, applying OR penalty.\n", nextPlayer->playerName);
        applyPenalty(nextPlayer, gameDeck, deckSize, 4); // Draw 4 cards
        return 0;
    }
}

int applyPenalty(player *p, card *deck, int *deckSize, int count) {
    if (*deckSize < count) {
        printf("Not enough cards left in the deck to draw.\n");
        count = *deckSize;
    }
    
    // Resize player's hand
    card *temp = realloc(p->deck, (p->decksize + count) * sizeof(card));
    if (!temp) {
        printf("Error reallocating memory for the penalty.\n");
        return 0;
    }
    p->deck = temp;
    
    // Draw from the end of the deck (top)
    for (int i = 0; i < count; i++) {
        p->deck[p->decksize + i] = deck[*deckSize - count + i];
    }
    p->decksize += count;
    *deckSize -= count;
    
    printf("%s draws %d cards\n", p->playerName, count);
    return count;
}

// Handle effect of NOT card (aka SKIP card).
int handleNOT(int currentPlayerIndex, int playerCount) {

    // if it is just 2 players - the skip card player gets another turn
    if (playerCount == 2) {
        return (currentPlayerIndex - 2);
    }
    
    printf("Next Player's turn skipped.\n");
    currentPlayerIndex = (currentPlayerIndex + 2) % playerCount;
    return currentPlayerIndex;
}

// Handle effect of Reverse card (reverse the turn order or in a two-player game, let current player go again
// FIXME: reverses order for one turn but then it goes back to normal
void handleReverse(int *reverseFlag, int *currentPlayerIndex, int playerCount) {

    // game with 2 players, no index change needed
    if (playerCount == 2) {
        return;
    }

    if (*reverseFlag == NORMAL_ORDER ) {
        *reverseFlag *= -1;  // reverse playing order 
    }
    else if (*reverseFlag == REVERSE_ORDER) {
        *reverseFlag *= -1;  // switch to normal playing order
    }
    
    // now move to the next player in the new playing order
    // 2 1 0 0 1 2 or 2 1 0 2 2 0 1
    if (*reverseFlag == NORMAL_ORDER ) {
        if (*currentPlayerIndex == (playerCount - 1)){  // come back to first player
            *currentPlayerIndex = 0;  
        }
        else {
            *currentPlayerIndex = *currentPlayerIndex + 1;
        }
    }
    // 0 1 2 0 0 2 1
    else if (*reverseFlag == REVERSE_ORDER) {
        if (*currentPlayerIndex == 0){
            *currentPlayerIndex = playerCount - 1; // go to last player
        }
        else {
            *currentPlayerIndex = *currentPlayerIndex - 1;
        }
    }
}
