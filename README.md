Quick log for what I've done:

5/5:
1) made the pile that players play into, with test functions it adds cards to the top of the pile and then accurately reads whats at the top of the pile. a lot of these functions need to be fixed for the final code, since they're just set up for testing right now, but it works!
2) next step is probably isValidCard(); and fixing/finalizing all the functions that work with the linked list. everything should be good enough for right now, but i'll bet that while testing in the future something's gonna mess up and we'll have to change something.
3) came up with isValidCard(); but realized we probably need to add the function that allows users to input which card to play to test it. never code at 4:30 am

5/4:
1) removed some newlines in the printCard(); function to try and fix formatting issues (extra spaces after special cards), seemingly fixed it, this was probably my bad!
2) additionally messed with newlines in printPlayerHand(); and main's loop to get the player names to get the formatting closer to what's shown in the guidelines

*note: barring maybe the formatting, all the functions and the code itself seem to be working great so far :) i've only tested it with two people myself, so maybe i'll run some tests with more players in a bit to make sure it works?

**note: tested it with 6 and 12 players, all with different names, everything still works this way too!

5/1:
1) I switched "card deck[]" in initializeDeck(); and CreateDeck(); to "card *deck", in short, switching from an array to a pointer. The function still works. 
2) Fixed the shuffleDeck(); function for now, it was returning a memory error when the bounds were "i < MAX_SHUFFLE" instead of "i < MAX_CARDS". 
3) Added an outer loop for shuffleDeck(); to repeat the shuffle the required 10,000 times... I think it should work?
4) In "ece101 5_1 second_file", attempted to try and get the printPlayerHand(); working. Got something printed, but it's not what I wanted... included a few fixme comments for later.

4/28: Tentatively got initializeDeck(); and printCard(); working, attempted to work on printPlayerHand(); but didn't get it working


