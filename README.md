Quick log for what I've done:

4/28: Tentatively got initializeDeck(); and printCard(); working, attempted to work on printPlayerHand(); but didn't get it working

5/1:
1) I switched "card deck[]" in initializeDeck(); and CreateDeck(); to "card *deck", in short, switching from an array to a pointer. The function still works. 
2) Fixed the shuffleDeck(); function for now, it was returning a memory error when the bounds were "i < MAX_SHUFFLE" instead of "i < MAX_CARDS". 
3) Added an outer loop for shuffleDeck(); to repeat the shuffle the required 10,000 times... I think it should work?
4) In "ece101 5_1 second_file", attempted to try and get the printPlayerHand(); working. Got something printed, but it's not what I wanted... included a few fixme comments for later.

5/4:
1) removed some newlines in the printCard(); function to try and fix formatting issues (extra spaces after special cards), seemingly fixed it, this was probably my bad!
2) additionally messed with newlines in printPlayerHand(); and main's loop to get the player names to get the formatting closer to what's shown in the guidelines
*note: barring maybe the formatting, all the functions and the code itself seem to be working great so far :) i've only tested it with two people myself, so maybe i'll run some tests with more players in a bit to make sure it works?
**note: tested it with 6 and 12 players, all with different names, everything still works this way too!
