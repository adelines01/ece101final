Quick log for what I've done:

4/28: Tentatively got initializeDeck(); and printCard(); working, attempted to work on printPlayerHand(); but didn't get it working

5/1:
1) I switched "card deck[]" in initializeDeck(); and CreateDeck(); to "card *deck", in short, switching from an array to a pointer. The function still works. 
2) Fixed the shuffleDeck(); function for now, it was returning a memory error when the bounds were "i < MAX_SHUFFLE" instead of "i < MAX_CARDS". 
3) Added an outer loop for shuffleDeck(); to repeat the shuffle the required 10,000 times... I think it should work?
