/*Author	: Catherine Lee
  Email		: catherine1995@shaw.ca
  Date		: July 9, 2014
  Purpose	: To allow users to play a single-player version of Blackjack, as outlined by http://en.wikipedia.org/wiki/Blackjack. 
			  Hit, Stand, Double Down, and Split functions are included. Insurance, instead of surrender is available.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

/*
-----------------------------------------------------------------
	Global Variables
-----------------------------------------------------------------
*/

//Define bet and hand values
#define MIN_BET 1
#define MAX_HAND 11
#define NUM_CARDS 13
#define NUM_CHIPS 100

//Define time variables where 1000 ms = 1 second
#define QUARTER_SEC 250 
#define FOUR_SEC 4000

//Define card specific values
#define MAX_ACE_VAL 11
#define FACE_VAL 10

#define ACE 1
#define JACK 11
#define QUEEN 12
#define KING 13

//Define Insurance option values
#define YES_INSURE 1
#define NO_INSURE 2

//Define Split options
#define YES_SPLIT 1
#define NO_SPLIT 2

//Hand values
#define FULL_HAND 21
#define EMPTY 0

//Deck values
#define MAX_NUM_PER_CARD 4

//Define action values
#define HIT 1
#define STAND 2
#define DOUBLE 3
#define SPLIT  4 //Must be the last option, as it may not be enabled.

#define FIRST_ACTION_VAL 1
#define LAST_ACTION_VAL 4
#define LAST_ACTION_VAL_WO_SPLIT 3

//Define story values
#define LOSE_STORY 1
#define WIN_STORY 2
#define ROUNDS_TO_WIN_STORY 5

//Define menu options
#define CASINO_MODE 1
#define STORY_MODE 2
#define RULES 3
#define QUIT 4

#define MIN_OPTION_VAL 1
#define MAX_OPTION_VAL 4

//Boolean Definitions
#define TRUE 1
#define FALSE 0

//Other global variables
#define OPENING_LINE_LENGTH 50
#define EXIT 0

/*
-----------------------------------------------------------------
	Functions Headers
-----------------------------------------------------------------
*/

//Functions to determine bet
double askUserBet(double remainingChips);
double checkWallet (double remainingChips);
double makeBet (double remainingChips);
int verifyBet(double betAmount, double remainingChips);

//Functions for initialization of hand
int openCard (int deck[], int hand[]);

//Functions for basic card options
int checkCardExists(int deck[], int cardDrawn);
int drawCard(int deck[]);
int drawManualCard(int deck[]);
void hitCard (int deck[], int hand[], int numCards);

//Double specific Functions
int checkDoubleValidity(double bet, double chipsRemaining);

//Bust specific Functions
int checkBust(int handSum);

//Push specific Functions
void printPushMsg(int numSplits, int handNum, int player[]);
int pushCheck(int dealerSum, int playerSum);

//Black Jack specific functions
double calculateWinForBlackJack(double bet, double chipsRemaining);
int checkBlackJack(int handSum, int numCards);
double printDealerBlackJack(double chipsRemaining, double bet);

//Insurance specific functions
int askInsurance(void);
int insuranceCheck(int dealer[]);
void printInsuranceDealerBJ(double chipsRemaining);
double processInsurance(int insurance, int dealer[], int dealerSum, double chipsRemaining, double bet);

//Functions for showing hand
void printCards(int hand[], int hideDealer);
void printDealerHidden(int dealer[]);
void printHandwithNum(int handNumber, int playerCurrentHand[]);
int printHandAndSum (int playerCurrentHand[], int numCardsinHand, int handNum, int afterSplit);
void showFullDealerHand(int dealer[], int dealerSum);

//Functions for determining and calculating win/lose results
double calculateRegWinLoss(int playerSum, int dealerSum, int numPlayerCards, int numDealerCards, double chipsRemaining, double bet, int activateDouble);
double calculateWinnings(int playerSum, int dealerSum, int numPlayerCards, int playerCurrentHand[], int numDealerCards, double chipsRemaining, double bet, int numSplits, int handNumber, int activateDouble);
int checkWin(int playerSum, int dealerSum, int playerHandSum, int dealerHandSum);
double determineResult(int dealerSum, int playerSum, double chipsRemaining, int numPlayerCards, int player[], int numDealerCards, double bet, int numSplits, int handNum, int activateDouble); 

//Functions for  summing hands
int addAcetoCardSum(int currentSum, int numAces);
int caclulateAces(int hand[], int numCards);
int sumCards(int hand[], int numCards);
int sumNumberCards(int hand[], int numCards);

//Split specific functions
int checkSplit(int numCardsinHand, int playerCurrentHand[], int splitEnable);
void performSplit(int currentHand[], int nextHand[], int deck[]);

//Player and Dealer Turns
int completePlayerHand(int deck[], int player[], int numPlayerCards, int nextPossibleHand[], int splitEnable, int handNum, int showSplitHand, int numSplits, double bet, double chipsRemaining);
int dealerTurn(int deck[], int dealer[], int numDealerCards, int dealerSum);
int pickAction(int deck[], int playerCurrentHand[], int numCardsinHand, int nextPossibleHand[], int splitEnable, int printAfterSplit, int handNum, double bet, double chipsRemaining);
double playerAndDealerTurns(int numPlayerCards, int numDealerCards, int playerSum, int dealerSum, int blackJack, int insurance, double chipsRemaining, int deck[], int player[], int dealer[], double bet);

//Functions to obtain user response
int askUserChoice(int numCardsinHand, int playerCurrentHand[], int splitEnable);

//Functions to process user response
int confirmUserChoiceAfterSplit(int deck[], int player[], int numPlayerCards, int playerSum, int nextPossibleHand[], int splitEnable, int showSplitHand, int handNum, int numSplits, double bet, double chipsRemaining);
void processUserAction(int userChoice, int deck[], int playerCurrentHand[], int numCardsinHand, int printAfterSplit, int handNum, int nextPossibleHand[]);
int verifyUserChoice(int userChoice, int numCardsinHand, int playerCurrentHand[], int splitEnable, double bet, double chipsRemaining);

//General functions for the main menu
int askMenuOptions(void);
int casinoMode(void);
void mainMenu(void);
void rules(void);

//Story Excerpts
void badEnd(void);
void goodEnd(void);
void storyIntroduction(void);
void storyMirrorWorldIntro(void);

//Executing Story Mode
int checkResultSMode(double remainingChips, double previousChipAmount);
int executeGameinStoryMode(void);
void processStoryResult(int gameResult);
void storyMode(void);

//Other functions
void initializeArray (int arrayToInit[], int maxIndex, int initVal);
void printLine(void);
double runGame (double chips, double bet);

/*
-----------------------------------------------------------------
	Functions to calculate bet
-----------------------------------------------------------------
*/

/*
Name: askUserBet
Param(s): remainingChips - the number of chips the player has left
Returns:  betAmount - the bet made by the user
Purpose: Asks the user for a bet
*/

double askUserBet(double remainingChips)
{
		double betAmount;

		printf("You have %.2lf chips. How much would you like to bet? Bet %d to leave the game.\n", remainingChips, EXIT);
		printf("\n");
		scanf("%lf", &betAmount);
		printf("\n");

		return betAmount;
}

/*
Name: checkWallet
Param(s): remainingChips - the number of chips the player has left
Returns: TRUE - if the player has chips remaining, FALSE otherwise
Purpose: Checks to see if the player has enough chips to make a new bet, and prints the number of chips remaining.
*/

double checkWallet (double remainingChips)
{
	if(remainingChips > 0)
	{
		return TRUE;
	}

	printf("You have no chips left! Better luck next time.\n");
	printf("\n");
	return FALSE;
}


/*
Name: makeBet
Param(s): remainingChips - the number of chips the player has
Returns: betAmount - the bet made by the user
Purpose: Fully enables a user to make a bet.
*/

double makeBet (double remainingChips)
{
	double betAmount;
	int validAmount; 
	
	do
	{
		betAmount = askUserBet(remainingChips);
		validAmount = verifyBet(betAmount, remainingChips);

	}while(validAmount == FALSE && betAmount != EXIT);

	return betAmount;
}


/*
Name: verifyBet
Param(s): betAmount - the bet entered by the user, remainingChips - amount of chips the user has left
Returns: TRUE if the bet entered was valid, FALSE otherwise
Purpose: Determines the validity of the bet entered.
*/

int verifyBet(double betAmount, double remainingChips)
{
		if(betAmount > remainingChips)
		{
			printf("You cannot bet more than the chips you have!\n");
			return FALSE;
		}

		else if(betAmount < MIN_BET && betAmount != EXIT)
		{
			printf("The minimum bet is %d chip(s). Please place a bet of equal or higher value.\n", MIN_BET);
			return FALSE;
		}

		else if(betAmount == EXIT)
		{
			return FALSE;
		}

		return TRUE;

}

/*
-----------------------------------------------------------------
	Functions for Initialization of Hand(s)
-----------------------------------------------------------------
*/

/*
Name: openCard
Param(s): deck[] - the card deck, hand[] - the player's hand
Returns: hand[] - the updated player hand, deck[] - the updated deck, handIndex - the number of cards placed in deck and hand (should always be 2)
Purpose: Initializes either the dealer or the player's hand
*/

int openCard (int deck[], int hand[])
{
		int handIndex = 0;

		while(handIndex < 2)
		{
			hitCard(deck, hand, handIndex);
			handIndex++;
		}

		return handIndex;
}

/*
-----------------------------------------------------------------
	Functions for Basic Card Options
-----------------------------------------------------------------
*/

/*
Name: checkCardExists
Param(s): deck[] - the card deck, cardDrawn - numerical worth of the card chosen
Returns: TRUE if card exists in deck, FALSE otherwise, deck[] - the updated deck
Purpose: Determines if the card is still available in the deck, and removes it if it is.
*/

int checkCardExists(int deck[], int cardDrawn)
{
	if(deck[cardDrawn] > 0)
		{
			deck[cardDrawn] -= 1; //Remove the card from the deck if it exists.
			return TRUE;
		}

	return FALSE;
}

/*
Name: drawCard
Param(s): deck[] - the current deck
Returns: cardDrawn - the numerical indicator of a card in the deck.
Purpose: Randomly chooses and removes a card from the deck.
*/

int drawCard(int deck[])
{
	int cardDrawn;
	int existingCard; 


	do //Start random selection
	{
		cardDrawn = rand()%12; //Select a card from 13 different possible cards.
		existingCard = checkCardExists(deck, cardDrawn);
			
	}while(existingCard == FALSE);  //End random selection -- comment out random selection and replace with a call to drawManualCard to activate manual draw mode.

	cardDrawn++;
	return cardDrawn;
}

/*
Name: drawManualCard
Param(s): deck[] - the deck
Returns: cardDrawn - the card chosen
Purpose: Allows the user to select what card is drawn. For debugging only. Replace random selection with a call to drawManualCard to use.
*/

int drawManualCard(int deck[])
{
	int cardDrawn;
	int existingCard;

	do{
		printf("What card would you like to draw? (Pick a number between 0-12. Number + 1 corresponds to card.)\n");
		scanf("%d", &cardDrawn);
		printf("\n");
		existingCard = checkCardExists(deck, cardDrawn);

	}while(existingCard == FALSE);

	return cardDrawn;
}

/*
Name: hitCard
Param(s): deck[] - the deck, hand[] - the player's hand, numCards - the number of cards in the hand
Returns: hand[]  - the updated player hand
Purpose: Allows either the player or dealer to hit (draw) a card.
*/

void hitCard (int deck[], int hand[], int numCards)
{
	int cardVal;
	cardVal = drawCard(deck);
	hand[numCards] = cardVal;

}

/*
-----------------------------------------------------------------
	Double Specific Functions
-----------------------------------------------------------------
*/


/*
Name: checkDoubleValidity
Param(s): bet - the bet made, chipsRemaining - the player chip total
Returns: TRUE if the player has enough chips to select double, FALSE otherwise
Purpose: Determines if the player has enough chips to perform a double down.
*/

int checkDoubleValidity(double bet, double chipsRemaining)
{
	if(bet*2 > chipsRemaining)
	{
		printf("You don't have enough chips to select Double Down! Please choose a different option.\n");
		printf("\n");
		return FALSE;
	}

	return TRUE;
}

/*
-----------------------------------------------------------------
	Bust Specific Functions
-----------------------------------------------------------------
*/

/*
Name: checkBust
Param(s): handSum - total value of cards in hand
Returns: TRUE if the hand is bust (> 21), FALSE otherwise
Purpose: Determines whether or not a hand is bust
*/

int checkBust(int handSum)
{
	if(handSum > FULL_HAND)
	{
		return TRUE;
	}

	return FALSE;
}


/*
-----------------------------------------------------------------
	Push Specific Functions
-----------------------------------------------------------------
*/


/*
Name: printPushMsg
Param(s): numSplits - the number of splits made so far, handNum - the current hand being played, player[] - the player's current hand
Returns: -
Purpose: Prints a message if a push has occured. For split hands, also shows the hand being calculated.
*/

void printPushMsg(int numSplits, int handNum, int player[])
{
		if(numSplits > 0)
		{
			printHandwithNum(handNum, player);
		}
		
		printf("Push! No win or loss for this hand!\n");
		printf("\n");
}


/*
Name: pushCheck
Param(s): dealerSum - the value of the dealer's hand, playerSum - the value of the player's hand
Returns: TRUE if the cards are equal in value, FALSE otherwise.
Purpose: Determines if a push has occured during the game.
*/

int pushCheck(int dealerSum, int playerSum)
{
	if(dealerSum == playerSum)
	{
		return TRUE;
	}

	return FALSE;
}

/*
-----------------------------------------------------------------
	Black Jack Specific Functions
-----------------------------------------------------------------
*/

/*
Name: calculateWinForBlackJack
Param(s): bet - the bet amount for the game, chipsRemaining - the total of the user's chips
Returns: chipsRemaining - the updated chip total
Purpose: Displays a win message and calculates the winnings from a player Black Jack.
*/

double calculateWinForBlackJack(double bet, double chipsRemaining)
{
	printf("Black Jack! You win 3 to 1!\n");
	printf("\n");
	
	chipsRemaining = chipsRemaining + 1.5*bet; //Black Jacks pay 3 to 1
	return chipsRemaining;
}

/*
Name: checkBlackJack
Param(s): handSum - the total value of cards in a hand, numCards - the number of cards in the hand.
Returns: TRUE if the hand is a Black Jack, FALSE otherwise
Purpose: Determines if a hand is a Black Jack
*/

int checkBlackJack(int handSum, int numCards)
{
	if(numCards == 2 && handSum == FULL_HAND)
	{
		return TRUE;
	}

	return FALSE;

}

/*
Name: printDealerBlackJack
Param(s): chipsRemaining - the player's total chips, bet - the bet made by the player
Returns: chipsRemaining - the updated chip total
Purpose: Displays the message printed and calculates the loss from a Dealer's Black Jack
*/

double printDealerBlackJack(double chipsRemaining, double bet)
{
	printf("Dealer's Black Jack! You lose.\n"); 
	printf("\n");
	chipsRemaining = chipsRemaining - bet; //Total loss of the dealer has a Black Jack.
	printf("You now have %.2lf chips.\n", chipsRemaining);
	printf("\n");

	return chipsRemaining;
}


/*
-----------------------------------------------------------------
	Insurance Specific Functions
-----------------------------------------------------------------
*/

/*
Name: askInsurance
Param(s): -
Returns: insuranceChoice - the user's decision 
Purpose: Ask the user if they would like to buy insurance.
*/

int askInsurance(void)
{
		int insuranceChoice;

		printf("Would you like to buy insurance against a dealer Black Jack?\n");
		printf("\n");
		printf("%d. Yes\n", YES_INSURE);
		printf("%d. No\n", NO_INSURE);

		printf("\n");
		scanf("%d", &insuranceChoice);
		printf("\n");

		return insuranceChoice;

}

/*
Name: insuranceCheck
Param(s): dealer[] - the dealer's hand
Returns: TRUE if conditions for buying insurance are met, FALSE otherwise
Purpose: Allows a player to purchase insurance 
*/

int insuranceCheck(int dealer[])
{
	int insuranceChoice;

	if(dealer[0] == ACE) //Black Jack suspected only when the revealed dealer card is an A
	{
		insuranceChoice = askInsurance();

		if(insuranceChoice == YES_INSURE)
		{
			return TRUE;
		}
		
	}

	return FALSE;
}

/*
Name: printInsuranceDealerBJ
Param(s): chipRemaining - the total number of chips the user has remaining
Returns: -
Purpose: Prints a message for players who purchased insurance for a Dealer's Black Jack.
*/

void printInsuranceDealerBJ(double chipsRemaining)
{
	printf("Dealer's Black Jack! Insurance came in handy!\n");
	printf("\n");
	printf("You now have %.2lf chips.\n", chipsRemaining);
	printf("\n");
}

/*
Name: processInsurance
Param(s): insurance - TRUE or FALSE depending on whether insurance was purchased, dealer[] - the dealer hand, 
		  dealerSum - the dealer's card value total, chipsRemaining - the total chips the user has left, bet - the bet made by the user
Returns: chipsRemaining - the updated chip amount
Purpose: Updates the chip total depending on whether or not insurance was in effect during a Dealer's Black Jack.
*/

double processInsurance(int insurance, int dealer[], int dealerSum, double chipsRemaining, double bet)
{
	showFullDealerHand(dealer, dealerSum);

	if(!insurance)
	{
		chipsRemaining = printDealerBlackJack(chipsRemaining, bet);
	}

	else
	{
		printInsuranceDealerBJ(chipsRemaining);
	}

	return chipsRemaining;
}

/*
-----------------------------------------------------------------
	Functions for Showing Hand(s)
-----------------------------------------------------------------
*/

/*
Name: printCards
Param(s): hand[] - the player's hand, hideDealer - TRUE if the dealer needs to hide his cards, FALSE otherwise
Returns: -
Purpose: Reveals the cards in a hand.
*/

void printCards(int hand[], int hideDealer)
{
	int index = 0;
	int checkHideDealer;
	checkHideDealer = FALSE;

	while(hand[index] > EMPTY && !checkHideDealer) //0 denotes an empty card slot, so the loop will check for cards and print accordingly. 
	{

		if(hand[index] == ACE)
		{
			printf(" A ");
		}

		else if(hand[index] == 2)
		{
			printf(" 2 ");
		}

		else if(hand[index] == 3)
		{
			printf(" 3 ");
		}

		else if(hand[index] == 4)
		{
			printf(" 4 ");
		}
		
		else if(hand[index] == 5)
		{
			printf(" 5 ");
		}

		else if(hand[index] == 6)
		{
			printf(" 6 ");
		}
		
		else if(hand[index] == 7)
		{
			printf(" 7 ");
		}

		else if(hand[index] == 8)
		{
			printf(" 8 ");
		}
		
		else if(hand[index] == 9)
		{
			printf(" 9 ");
		}

		else if(hand[index] == 10)
		{
			printf(" 10 ");
		}
		
		else if(hand[index] == JACK)
		{
			printf(" Jack ");
		}

		else if(hand[index] == QUEEN)
		{
			printf(" Queen ");
		}

		else if(hand[index] == KING)
		{
			printf(" King ");
		}

		index++;
		checkHideDealer = hideDealer; //Ensures that only the first dealer card will be shown.

	}

	while(hand[index] > 0) //Hides any cards beyond the first.
	{
		printf(" ??? ");
		index++;
	}

	printf("\n");

}

/*
Name: printDealerHidden
Param(s): dealer[] - the dealer's hand
Returns: -
Purpose: Prints dealer's hand while it is the player's turn.
*/

void printDealerHidden(int dealer[])
{
	printf("Dealer's hand:");
	printCards(dealer, TRUE);
	printf("\n");

}

/*
Name: printHandwithNum
Param(s): handNumber - the numerical standing of the hand for split hands, playerCurrentHand[] - the hand being played
Returns: - 
Purpose: Shows any hand of choice for split hands.
*/

void printHandwithNum(int handNumber, int playerCurrentHand[])
{
		printf("Hand %d:", handNumber);
		printCards(playerCurrentHand, FALSE);
		printf("\n");
}


/*
Name: printHandAndSum
Param(s): playerCurrentHand[] - the hand being played, numCardsinHand - the number of cards in that hand, 
	      handNum - the numerical standing of the hand, afterSplit - TRUE if a split had previously occured, FALSE otherwise
Returns: handSum - the updated sum of cards in the hand.
Purpose: Display's the player's hand and its sum.
*/

 
int printHandAndSum (int playerCurrentHand[], int numCardsinHand, int handNum, int afterSplit)
{
		int handSum;
		handSum = sumCards(playerCurrentHand, numCardsinHand);

		if(afterSplit)
		{
			printHandwithNum(handNum, playerCurrentHand);
		}
		else
		{
			printf("Your Hand:");
			printCards(playerCurrentHand, FALSE);
			printf("\n");
		}

		printf("Sum: %d\n", handSum);
		printf("\n");

		return handSum;

}

/*
Name: showFullDealerHand
Param(s): dealer[] - the dealer's hand, dealerSum - the total value of the dealer's cards
Returns: -
Purpose: Shows the dealer's hand without hiding any cards.
*/

void showFullDealerHand(int dealer[], int dealerSum)
{
	printf("Dealer's Hand:");
	printCards(dealer, FALSE);
	printf("\n");
	printf("Sum: %d\n", dealerSum);
		
	printf("\n");
}


/*
-----------------------------------------------------------------
	Functions for calculating results from win(s)/loss(es)
-----------------------------------------------------------------
*/

/*
Name: calculateRegWinLoss
Param(s): playerSum - the player hand's total card value, dealerSum - the dealer hand's total card value, 
		  numPlayerCards - the number of player cards, numDealerCards- the number the dealer's cards, 
		  chipsRemaining - the player's remaining chips, bet - the bet made, activateDouble - TRUE if a double had occured, FALSE otherwise
Returns: chipsRemaining - the updated chip total
Purpose: Determine a win or loss if neither Dealer or Player has a Black Jack or Bust.
*/

double calculateRegWinLoss(int playerSum, int dealerSum, int numPlayerCards, int numDealerCards, double chipsRemaining, double bet, int activateDouble)
{
	
	if(checkWin(playerSum, dealerSum, numPlayerCards, numDealerCards))
	{
		printf("You win with %d!\n", playerSum);
		chipsRemaining = chipsRemaining + bet + activateDouble*bet; //If activate Double is TRUE, the bet is doubled. 
	}

	else
	{
		printf("You lose with %d.\n", playerSum);
		chipsRemaining = chipsRemaining - bet - activateDouble*bet; //If activate Double is TRUE, the bet is doubled. 
	}

	printf("\n");
	return chipsRemaining;
 		
}

/*
Name: calculateWinnings
Param(s): playerSum - the player's current hand value, dealerSum - the dealer's current hand value, 
	      numPlayerCards - the number of cards in the player's current hand, playerCurrentHand[] - the player's current hand, 
		  numDealerCards - the number of cards in the dealer's hand, chipsRemaining -  the player's chip total, bet - the bet placed in the game, 
		  numSplits - number of splits, handNumber - hand number for multiple hands, activateDouble - TRUE if the hand was doubled, FALSE otherwise.
Returns: chipsRemaining - the updated chip total
Purpose: Determines the chips remaining after accounting for the win/loss of a player's hand(s).
*/


double calculateWinnings(int playerSum, int dealerSum, int numPlayerCards, int playerCurrentHand[], int numDealerCards, double chipsRemaining, double bet, int numSplits, int handNumber, int activateDouble)
{
	if(numSplits > 0)
	{
		printHandwithNum(handNumber, playerCurrentHand);

		if(checkBlackJack(playerSum, numPlayerCards))
		{
			chipsRemaining = calculateWinForBlackJack(bet, chipsRemaining);
			return chipsRemaining;
		}

	}

	chipsRemaining = calculateRegWinLoss(playerSum, dealerSum, numPlayerCards, numDealerCards, chipsRemaining, bet, activateDouble);

	return chipsRemaining;
}

/*
Name: checkWin
Param(s): playerSum - player hand's value, dealerSum - the dealer's hand value, numPlayerCards - the number of cards on hand, 
	      numDealerCards - the number of dealer's cards
Returns: TRUE if the player wins, FALSE if dealer wins
Purpose: Determines whether or not the player won that game
*/

int checkWin(int playerSum, int dealerSum, int numPlayerCards, int numDealerCards)
{

	 if(checkBlackJack(playerSum, numPlayerCards) || checkBust(dealerSum)) //Instant win if player has a Black Jack or dealer busts.
	 {
		 if(checkBust(dealerSum))
		 {
			 printf("Dealer Bust! ");

		 }

		 if(!checkBust(playerSum))
		 {
			 return TRUE;
		 }

		 else
		 {
			 printf("But your hand is bust as well. ");
		 }
	 }

	else if(playerSum > dealerSum && !checkBust(playerSum) && !checkBust(dealerSum)) //Otherwise the higher card holder wins, so long as neither busts.
	{
		return TRUE;
	}

	else if(checkBust(playerSum))
	{
		printf("Bust! ");
	}

	return FALSE;
}

/*
Name: determineResult
Param(s): dealerSum - the dealer's hand value, playerSum - the player's hand value, chipsRemaining - the player's chip total, 
	      numPlayerCards - number of cards in the player's hand, player[] - player's hand, 
		  numDealerCards - the number of dealer cards, bet - the bet made, numSplits - current number of splits, 
		  handNum - hand number for multiple hands, activateDouble - TRUE if a double has occured in the hand, FALSE otherwise
Returns: chipsRemaining - the number of chips remaining.
Purpose: Calculates whether a player wins or not for any hand of choice.
*/

double determineResult(int dealerSum, int playerSum, double chipsRemaining, int numPlayerCards, int player[], int numDealerCards, double bet, int numSplits, int handNum, int activateDouble)
{
	if(pushCheck(dealerSum, playerSum) && !checkBust(playerSum) && !checkBust(dealerSum))
	{
			printPushMsg(numSplits, handNum, player);
	}

	//Check if player wins if there was no draw.

	else if(numPlayerCards > 0)
	{
		chipsRemaining = calculateWinnings(playerSum, dealerSum, numPlayerCards, player, numDealerCards, chipsRemaining, bet, numSplits, handNum, activateDouble);
	}

	return chipsRemaining;
}

/*
-----------------------------------------------------------------
	Functions to Sum Hands
-----------------------------------------------------------------
*/

/*
Name: addAcetoCardSum
Param(s): currentSum - the current hand sum, numAces - the number of aces in the hand.
Returns: finalSum - the total hand value
Purpose: Calculates the hand's sum, including aces with their appropriate value 
	    (either 1 or 11 depending on whether or not sum will result in a bust with the higher option.)
*/

int addAcetoCardSum(int currentSum, int numAces)
{
	int finalSum;

	if((currentSum + numAces*MAX_ACE_VAL < FULL_HAND + 1))
	{
		finalSum = currentSum + numAces*MAX_ACE_VAL;
	}

	else
	{
		finalSum = currentSum + numAces;
	}

	return finalSum;
}

/*
Name: calculateAces
Param(s): hand[] - the player's current hand, numCards - the number of cards in the hand.
Returns: numAces - the number of aces in the hand.
Purpose: Calculates the number of aces in the player's hand.
*/

int caclulateAces(int hand[], int numCards)
{
	int numAces;
	int currentIndex;

	numAces = 0;

	for(currentIndex = 0; currentIndex < numCards; currentIndex++)
	{
		if(hand[currentIndex] == ACE)
		{
			numAces++;
		}

	}

	return numAces;
}

/*
Name: sumCards
Param(s): hand[] - the hand to sum, numCards - the number of cards in the hand.
Returns: sumVal - the most current sum of cards
Purpose: Calculates the total value of cards.
*/

int sumCards(int hand[],int numCards)
{
	int numAces; //Track the nuber of Aces in the hand, since A may take 1 or 11 in value.
	int sumVal; //Tracks the card total
	
	numAces = caclulateAces(hand, numCards);
	sumVal = sumNumberCards(hand, numCards);
	
	sumVal = addAcetoCardSum(sumVal, numAces);

	return sumVal;

}

/*
Name: sumNumberCards
Param(s): hand[] - the hand to sum, numCards - the number of cards in that hand
Returns: cardSum - the sum of number cards
Purpose: Calculate a temporary sum for cards in a hand with a set value.
*/

int sumNumberCards(int hand[], int numCards)
{
	int cardSum;
	int currentIndex;

	cardSum = 0;

	for(currentIndex = 0; currentIndex < numCards; currentIndex++)
	{
		if(hand[currentIndex] > ACE && hand[currentIndex] < 10)
		{
			cardSum += hand[currentIndex];
		}

		else if(hand[currentIndex] > 9)
		{
			cardSum += FACE_VAL; 

		}
	}

	return cardSum;
}


/*
-----------------------------------------------------------------
	Split Specific Functions
-----------------------------------------------------------------
*/

/*
Name: checkSplit
Param(s): numCardsinHand - the number of cards in the player's hand, playerCurrentHand - the active hand being played,
		  splitEnable - TRUE if splits are allowed to occur, FALSE otherwise
Returns: TRUE if the conditions for a split are met, FALSE otherwise
Purpose: Determine whether or not split is to be made available.
*/

int checkSplit(int numCardsinHand, int playerCurrentHand[], int splitEnable)
{
	if(numCardsinHand == 2 && playerCurrentHand[0] == playerCurrentHand[1] && splitEnable == TRUE) //Allow split only if  two of the same cards are available.
	{
		return TRUE;
	}

	return FALSE;


}

/*
Name: performSplit
Param(s): currentHand[] - the current player hand, nextHand[] - the next possible hand to use, deck[] - the current deck
Returns: currentHand[] - the updated current hand, newHand[] - the updated new hand
Purpose: Removes a card from the player's current hand and places it in a new hand. An additional card is drawn for both hands to open the hands.
*/

void performSplit(int currentHand[], int nextHand[], int deck[])
{
	nextHand[0] = currentHand[1];
	currentHand[1] = drawCard(deck);
	nextHand[1] = drawCard(deck);
}

/*
-----------------------------------------------------------------
	Player and Dealer Turns
-----------------------------------------------------------------
*/

/*Name: completePlayerHand
Param(s): deck[] - the current player deck, player[] - the current player hand, numPlayerCards - the number of cards in the players hand, 
		  nextPossibleHand - the next player hand for a split, 
		  splitEnable - whether or not the user can choose to split if split conditions are met, 
		  handNum - the hand number for multiple hands, showSplitHand - TRUE if splits have occured, FALSE otherwise, 
		  numSplits - the number of splits that have occured, bet - the bet made, chipsRemaining - the player chip total
Returns: numPlayerCards - the number of cards after the hand is complete
Purpose: Allows the player to decide what to do with his cards.
*/

int completePlayerHand(int deck[], int player[], int numPlayerCards, int nextPossibleHand[], int splitEnable, int handNum, int showSplitHand, int numSplits, double bet, double chipsRemaining)
{
	int playerSum;

	playerSum = sumCards(player, numPlayerCards); 

	if(checkBlackJack(playerSum, numPlayerCards))
	{
		printHandAndSum(player, numPlayerCards, handNum, TRUE); //Check for a Black Jack before offering player options
		printf("Black Jack for this hand!\n");
		printf("\n");
		
		return numPlayerCards;
	}

	numPlayerCards = confirmUserChoiceAfterSplit(deck, player, numPlayerCards, playerSum, nextPossibleHand, splitEnable, showSplitHand, handNum, numSplits, bet, chipsRemaining);
	
	return numPlayerCards;
}

/*
Name: dealerTurn
Param(s): deck[] - the deck, dealer[] - the dealer's hand, numDealerCards - number of cards of the dealer, 
		  dealerSum - the total value of dealer cards
Returns: numDealerCards - the number of cards the dealer
Purpose: Allows the dealer to play its turn
*/

int dealerTurn(int deck[], int dealer[], int numDealerCards, int dealerSum)
{
	showFullDealerHand(dealer, dealerSum);

	while(dealerSum < 17)
	{
		hitCard(deck, dealer, numDealerCards);
		numDealerCards++;
		dealerSum = sumCards(dealer, numDealerCards);

		showFullDealerHand(dealer, dealerSum);

		Sleep(QUARTER_SEC);

	}

	return numDealerCards;
}

/*
Name: pickAction
Param(s): deck[] - the current deck, playerCurrentHand[] - the hand being played, numCardsinHand - the number of cards in that hand, 
		  nextPossibleHand - the next hand for a split, splitEnable - allow a split to occur or not, 
		  printAfterSplit - TRUE if a split has occured, FALSE otherwise, handNum - the hand number for multiple hands,
		  bet - the bet made, chipsRemaining - the player chip total
Returns: userChoice - the user's selected action
Purpose: Allows the user to select to hit, stand, double, or if they have the same card in a two card hand, split.
*/

int pickAction(int deck[], int playerCurrentHand[], int numCardsinHand, int nextPossibleHand[], int splitEnable, int printAfterSplit, int handNum, double bet, double chipsRemaining)
{
	int userChoice;
	
	if(printAfterSplit)
	{
		printHandAndSum(playerCurrentHand, numCardsinHand, handNum, TRUE);
	}

	do{

		userChoice = askUserChoice(numCardsinHand, playerCurrentHand, splitEnable);

	} while(!verifyUserChoice(userChoice, numCardsinHand, playerCurrentHand, splitEnable, bet, chipsRemaining));

	processUserAction(userChoice, deck, playerCurrentHand, numCardsinHand, printAfterSplit, handNum, nextPossibleHand);
	
	return userChoice;

}

/*
Name: playerAndDealerTurns
Param(s): numPlayerCards - the number of cards in the player's hand, numDealerCards - the number of cards in the dealer's hand, 
		  playerSum - the total player hand card value, dealerSum - the total dealer card value,
		  blackJack - TRUE if a Black Jack occured, FALSE otherwise, insurance - TRUE if insurance was bought, FALSE otherwise, 
		  chipsRemaining - the player chip total, deck[] - the deck, dealer - the dealer's hand, bet - the bet made
Returns: chipsRemaining - the updated number of chips remaining
Purpose: Allow the player and dealer to both play after the initialization of the game. Results are calculated in the process.
*/
double playerAndDealerTurns(int numPlayerCards, int numDealerCards, int playerSum, int dealerSum, int blackJack, int insurance, double chipsRemaining, int deck[], int player[], int dealer[], double bet)
{
	//Declare tracking variables for possible splits.
	int numSplits;
	int splitsWon;

	//Other variables for possible splits
	int numPlayerCards2;
	int numPlayerCards3;
	int numPlayerCards4;

	int playerSum2;
	int playerSum3;
	int playerSum4;

	//Variables for possible doubles after splits
	int doubleHand1;
	int doubleHand2;
	int doubleHand3;
	int doubleHand4;

	//Declare four additional hands for possible splits.
	int playerHand2[MAX_HAND];
	int playerHand3[MAX_HAND];
	int playerHand4[MAX_HAND];


	//Initialization of split variables
	numSplits = 0;
	splitsWon = 0;
	numPlayerCards2 = 0;
	numPlayerCards3 = 0;
	numPlayerCards4 = 0;

	//Initialize double after split variables. By default there is no double.
	doubleHand1 = FALSE;
	doubleHand2 = FALSE;
	doubleHand3 = FALSE;
	doubleHand4 = FALSE;


	//initialize additional array
	initializeArray(playerHand2, MAX_HAND, 0);
	initializeArray(playerHand3, MAX_HAND, 0);
	initializeArray(playerHand4, MAX_HAND, 0);

	//Game begins with player's turn
	
	if(!blackJack)
	{

		if(numSplits == 0)
		{			
			numPlayerCards = completePlayerHand(deck, player, numPlayerCards, playerHand2, TRUE, 1, FALSE, numSplits, bet, chipsRemaining);
			playerSum2 = sumCards(playerHand2, 1); //Assume there is a card in the next array to test for splitting.

			if(numPlayerCards < 0) //Checks to see if double down has occured.
			{
				numPlayerCards *= -1; //Reverses indicator from before.
				doubleHand1 = TRUE;
			}

			else if(playerSum2 > 0) //The user cannot split after double.
			{
				numPlayerCards2 = 2;
				numSplits++;
			}

		}


		if(numSplits == 1)
		{
			numPlayerCards = 2;
			numPlayerCards = completePlayerHand(deck, player, numPlayerCards, playerHand2, FALSE, 1, TRUE, numSplits, bet, chipsRemaining); //Allow the user to complete the first hand before proceeding to the next.
			numPlayerCards2 = completePlayerHand(deck, playerHand2, numPlayerCards2, playerHand3, TRUE, 2, TRUE, numSplits, bet, chipsRemaining);
			playerSum3 = sumCards(playerHand3, 1);

			
			if(numPlayerCards  < 0) //Checks to see if double down has occured.
			{
				numPlayerCards  *= -1; //Reverses indicator from before;
				doubleHand1 = TRUE;
			}

			if(numPlayerCards2  < 0) //Checks to see if double down has occured.
			{
				numPlayerCards2  *= -1; //Reverses indicator from before;
				doubleHand2 = TRUE;
			}

			else if(playerSum3 > 0) //The user cannot split after double.
			{
				numPlayerCards3 = 2;
				numSplits++;
			}
		}

		if(numSplits == 2)
		{
			numPlayerCards2 = 2;
			numPlayerCards2 = completePlayerHand(deck, playerHand2, numPlayerCards2, playerHand3, FALSE, 2, TRUE, numSplits, bet, chipsRemaining); //Allow the user to complete the first hand before proceeding to the next.
			numPlayerCards3 = completePlayerHand(deck, playerHand3, numPlayerCards3, playerHand4, TRUE, 3, TRUE, numSplits, bet, chipsRemaining);
			playerSum4 = sumCards(playerHand4, 1);

			if(numPlayerCards2  < 0) //Checks to see if double down has occured.
			{
				numPlayerCards2  *= -1; //Reverses indicator from before;
				doubleHand2 = TRUE;
				
			}

			if(numPlayerCards3 < 0) //Checks to see if double down has occured.
			{
				numPlayerCards3 *= -1; //Reverses indicator from before.
				doubleHand3 = TRUE;
			}

			else if(playerSum4 > 0) //The user cannot split after double.
			{
				numPlayerCards4 = 2;
				numSplits++;
			}
		}

		if(numSplits == 3)
		{
			numPlayerCards3 = 2;
			numPlayerCards3 = completePlayerHand(deck, playerHand3, numPlayerCards3, playerHand4, FALSE, 3, TRUE, numSplits, bet, chipsRemaining); //Allow the user to complete the first hand before proceeding to the next.
			numPlayerCards4 = completePlayerHand(deck, playerHand4, numPlayerCards4, playerHand4, FALSE, 4, TRUE, numSplits, bet, chipsRemaining); 
			
			
			if(numPlayerCards3 < 0) //Checks to see if double down has occured.
			{
				numPlayerCards3 *= -1; //Reverses indicator from before.
				doubleHand3 = TRUE;
			}

			if(numPlayerCards4 < 0) //Checks to see if double down has occured.
			{
				numPlayerCards4 *= -1; //Reverses indicator from before.
				doubleHand4 = TRUE;
			}
		}
		
		playerSum = sumCards(player, numPlayerCards);
		playerSum2 = sumCards(playerHand2, numPlayerCards2);
		playerSum3 = sumCards(playerHand3, numPlayerCards3);
		playerSum4 = sumCards(playerHand4, numPlayerCards4);

		//Dealer's Turn
		if(!checkBust(playerSum) || (checkBust(playerSum) && numSplits > 0))
		{
			numDealerCards = dealerTurn(deck, dealer, numDealerCards, dealerSum);
			dealerSum = sumCards(dealer, numDealerCards);
		}

		//Check for hand result(s)

		chipsRemaining = determineResult(dealerSum, playerSum, chipsRemaining, numPlayerCards, player, numDealerCards, bet, numSplits, 1, doubleHand1);
		chipsRemaining = determineResult(dealerSum, playerSum2, chipsRemaining, numPlayerCards2, playerHand2, numDealerCards, bet, numSplits, 2, doubleHand2);
		chipsRemaining = determineResult(dealerSum, playerSum3, chipsRemaining, numPlayerCards3, playerHand3, numDealerCards, bet, numSplits, 3, doubleHand3);
		chipsRemaining = determineResult(dealerSum, playerSum4, chipsRemaining, numPlayerCards4, playerHand4, numDealerCards, bet, numSplits, 4, doubleHand4);
		
	}	

	return chipsRemaining;

}

/*
-----------------------------------------------------------------
	Functions to Obtain User Response
-----------------------------------------------------------------
*/

/*
Name: askUserChoice
Param(s): numCardsinHand - the number of cards in the player's hand, playerCurrentHand - the active hand being played,
		  splitEnable - TRUE if splits are allowed to occur if conditions are met, FALSE otherwise
Returns: userChoice - the user's selected response
Purpose: To allow the user to select what to do with his cards.
*/

int askUserChoice(int numCardsinHand, int playerCurrentHand[], int splitEnable)
{
		int userChoice;

		printf("What would you like to do?\n");
		printf("\n");
		printf("%d.Hit\n", HIT);
		printf("%d.Stand\n", STAND);
		printf("%d.Double\n", DOUBLE);

		if(checkSplit(numCardsinHand, playerCurrentHand, splitEnable)) 
		{
			printf("%d.Split\n", SPLIT);
		}

		printf("\n");
		
		scanf("%d", &userChoice);

		printf("\n");

		return userChoice;
}

/*
-----------------------------------------------------------------
	Functions for Processing User Action
-----------------------------------------------------------------
*/

/*
Name: confirmUserChoiceAfterSplit
Param(s): deck[] - the deck, player[] - the player's active hand, numPlayerCards - the active hand's number of cards
		  playerSum - the total card value of the player's hand, nextPossibleHand - the next hand for another potential split, 
		  splitEnable - TRUE if a split is permitted, FALSE otherwise, showSplitHand - TRUE if a split has previous occured, FALSE otherwise
		  handNum - the hand number for multiple hands, numSplits - the number of splits that have occured, bet - the bet made,
		  chipsRemaining - the player chip total
Returns: The number of player cards after the action
Purpose:  Processes the user's choice after a split, and performs the action specified.
*/


int confirmUserChoiceAfterSplit(int deck[], int player[], int numPlayerCards, int playerSum, int nextPossibleHand[], int splitEnable, int showSplitHand, int handNum, int numSplits, double bet, double chipsRemaining)
{
	int checkPlayerChoice;

	do
	{
		checkPlayerChoice = pickAction(deck, player, numPlayerCards, nextPossibleHand, splitEnable, showSplitHand, handNum, bet, chipsRemaining);
		
		if(checkPlayerChoice == HIT || checkPlayerChoice == DOUBLE)
		{
			numPlayerCards++;
			playerSum = sumCards(player, numPlayerCards);
			
			if(numSplits == 0)
			{
				printHandAndSum(player, numPlayerCards, handNum, numSplits); 
			}


			if(checkPlayerChoice == DOUBLE)
			{
				numPlayerCards *= -1; //Negative value used as an indicator of double, while preserving numerical value of cards.
				checkPlayerChoice = STAND; //Double only allows one hit.
			}
		
			else if(checkBust(playerSum))
			{
				if(numSplits > 0)
				{
					printHandAndSum(player, numPlayerCards, handNum, numSplits);
					printf("Bust!\n");
					printf("\n");
				}

				checkPlayerChoice = STAND; //Automatically terminate user's turn for a bust.
			}
			
			else if(checkPlayerChoice == SPLIT)
			{
				numPlayerCards = 1;
			}

		}

	}while((checkPlayerChoice == HIT)); //Only when players hit will they possibly have other possible options.

	return numPlayerCards;
}

/*
Name: processUserAction
Param(s): userChoice - the user's choice of what to do with his cards, deck[] - the deck, playerCurrentHand[] - the active hand being played
		  numCardsinHand - the number of cards in that hand, printAfterSplit - TRUE if a split has occured, FALSE otherwise, 
		  handNum - the hand number for multiple hands, nextPossibleHand[] - next hand to use if a split occurs
Returns:  playerCurrentHand[] - updated hand depending on action, nextPossibleHand - updated hand if SPLIT is chosen
Purpose: Determines what actions to execute based on the user's response.
*/

void processUserAction(int userChoice, int deck[], int playerCurrentHand[], int numCardsinHand, int printAfterSplit, int handNum, int nextPossibleHand[])
{
	if(userChoice == HIT || userChoice == DOUBLE)
	{
		hitCard(deck, playerCurrentHand, numCardsinHand);

		if(userChoice == DOUBLE && printAfterSplit)
		{
			numCardsinHand++;
			printHandAndSum(playerCurrentHand, numCardsinHand, handNum, TRUE);
		}
	}

	if(userChoice == SPLIT)
	{
		performSplit(playerCurrentHand, nextPossibleHand, deck);
		
	}

}

/*
Name: verifyUserChoice
Param(s): userChoice - the users selected action, numCardsinHand - the number of cards in the player's hand, 
		  playerCurrentHand[] - the player's hand, splitEnable - TRUE if split can occur, FALSE otherwise,
		  bet - the bet made, chipsRemaining - the user chip total
Returns: TRUE if the user selected a valid option, FALSE otherwise.
Purpose: Checks for whether or not the user selection was valid.
*/

int verifyUserChoice(int userChoice, int numCardsinHand, int playerCurrentHand[], int splitEnable, double bet, double chipsRemaining)
{

	if(userChoice > LAST_ACTION_VAL)
	{
		return FALSE;
	}

	else if(userChoice > LAST_ACTION_VAL_WO_SPLIT && !checkSplit(numCardsinHand, playerCurrentHand, splitEnable))
	{
		return FALSE;
	}

	else if(userChoice < FIRST_ACTION_VAL)
	{
		return FALSE;
	}

	else if(userChoice == DOUBLE && !checkDoubleValidity(bet, chipsRemaining))
	{
		return FALSE;
	}
		
	return TRUE;
}

/*
-----------------------------------------------------------------
	General Functions for the Main Menu
-----------------------------------------------------------------
*/

/*
Name: askMenuOptions
Param(s): -
Returns: The user's response
Purpose: Contiunually display the Main Menu until the user responds with a choice.
*/

int askMenuOptions(void)
{
	int userOption;

	printLine();
	printf("Welcome to Black Jack Through the Mirror!\n");
	printf("\n"); 
	printLine();

	printf("%d. Play in Casino Mode\n", CASINO_MODE);
	printf("%d. Play in Story Mode\n", STORY_MODE);
	printf("%d. How to Play\n", RULES);
	printf("%d. Exit\n", QUIT);
	printf("\n");

	do
	{
		scanf("%d", &userOption);

	}while(userOption < MIN_OPTION_VAL || userOption > MAX_OPTION_VAL);

	printf("\n");

	return userOption;
}

/*
Name: casinoMode
Param(s): -
Returns: bet -  check if the user has quit the game by not making a bet
Purpose: Continually runs the BlackJack game until the user quits or runs out of money
*/

int casinoMode(void)
{
	//Declare tracking variables
	double chips;
	double bet;
	chips = NUM_CHIPS;

	do
	{
		bet = makeBet(chips);
		
		if(bet == EXIT)
		{
			return (int)bet;
		}

		chips = runGame(chips, bet);

	} while(checkWallet(chips));

	return (int)bet;
}

/*
Name: mainMenu
Param(s): -
Returns: -
Purpose: Displays options for the player to choose from when the game is launched.
*/

void mainMenu(void)
{
	int userOption;
	int noExit;
	
	
	noExit = TRUE;

	do
	{
		userOption = askMenuOptions();

		if(userOption == CASINO_MODE)
		{
			 casinoMode();
		}

		else if (userOption == STORY_MODE)
		{
			storyMode();
		}

		else if(userOption == RULES)
		{
			rules();
		}

		else if (userOption == QUIT)
		{
			noExit = FALSE;
		}

	}while(noExit != FALSE);

}

/*
Name: rules
Param(s): -
Returns: -
Purpose: Displays instructions on how to play Black Jack.
*/

void rules(void)
{
	printf("At the onset of the game, both the player and the dealer will be given two cards each. All of the player's cards are visible, while only one of the dealer's is revealed.\n");
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("All number cards correspond to their numerical worth, though Ace cards are worth %d while the sum of cards is under %d, and %d otherwise.\n", MAX_ACE_VAL, FULL_HAND, ACE);
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("Face cards are all worth 10 points each.\n");
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("If either the player or the dealer has an Ace card paired with a card worth %d, they will win a Black Jack.\n", FACE_VAL);
	printf("\n");
	Sleep(FOUR_SEC);

	printf("For players, Black Jacks pay 3 to 1. If the dealer wins a Black Jack, the player simply loses the full bet.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("If the revealed card of the dealer is an Ace, players may choose to purchase Insurance with half of their bet to prevent a full loss.\n");
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("If neither win a Black Jack at the start, the game proceeds with the player's turn.\n");
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("The player may hit (draw an additional card), stand (finish his turn), or double (draw one card and finish his turn while doubling the bet.)\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("If the player is lucky enough to obtain two of the same card, he may choose to split, and have multiple hands to play.\n");
	printf("\n");
	Sleep(FOUR_SEC);	

	printf("Once the player finishes his turn, the dealer will play.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("If either the sum of the player's hand or the dealer's hand exceeds %d, a bust has occured, which is an automatic loss of that hand.\n", FULL_HAND);
	printf("\n");
	Sleep(FOUR_SEC);

	printf("If neither busts or obtain a Black Jack, the winner is simply the holder of the higher hand sum.\n");
	printf("\n");
}

/*
-----------------------------------------------------------------
	Story Content
-----------------------------------------------------------------
*/

/*
Name: badEnd
Param(s): -
Returns: -
Purpose: Display the story exerpt for a loss in story mode.
*/


void badEnd(void)
{
	printLine();

	printf("\"No...\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"It was a shame, little lady, that you were not the one to grant my wish.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("Disappointed, the witch walked off.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("Shirley tore off a handful of grass, throwing it in frustration.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("She had ruined what was possibly her only chance to return home.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Elly...I'm sorry, I couldn't keep my promise after all.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

}

/*
Name: goodEnd
Param(s): -
Returns: -
Purpose: Displays the story exerpt for a win in story mode.
*/


void goodEnd(void)
{
	printLine();

	printf("She had done it. Against all odds of the gamble, she had won.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"As I thought, you were the one to grant my wish,\" the witch smiled.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"It's my turn to complete our deal then. Close your eyes, little lady,\" she told her.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printLine();

	printf("\"Shirley?\" her sister's voice nearly brought tears to her eyes.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Elly! I'm back. I won't leave you ever again.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"I know you won't,\" she smiled.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"I know you always keep your promises to me,\" Eleanor reached out, embracing her sister tightly.\n");
	printf("\n");
	Sleep(FOUR_SEC);

}

/*
Name: storyIntroduction
Param(s): -
Returns: -
Purpose: Displays the story introduction for a user playing in story mode to read.
*/


void storyIntroduction(void)
{
	printLine();

	printf("Dear Diary,\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("Today, Elly and I have been sent away to our relatives.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("They don't care much for us, so long as we stay out of trouble.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("But Elly has fallen ill. The doctors fear that she her fever is severe. They are unsure if the illness will worsen.\n");
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("I can only hope that she will recover. We have already lost our parents. We cannot lose each other too.\n");
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("Closing the small book, Shirley glanced at her sister's sleeping form.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("Running her fingers through Eleanor's hair, she prayed to any deities out there to cure her.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("She vowed to always stay by Eleanor's side, and protect her from anything, especially after the accident.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("But there was little she could do against this enemy.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Shirley?\" Eleanor murmured, blinking at her hazily.\n");
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("\"Yeah, I'm here. Don't worry, Elly. Big sis is here.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);
	
	printf("Eleanor smiled at that, before frowning at the mirror on the wall.\n ");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Why is there a meadow inside the mirror?\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"A meadow?\" Shirley panicked at the thought of the fever making her sister delusional.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Yes.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"There's no meadow there, see?\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("She placed a hand on the mirror to prove her point.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Huh?\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("The mirror was not solid.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("And she could no longer remove her hand.\n");
	printf("\n");
	Sleep(FOUR_SEC);

}

/*
Name: storyMirrorWorldIntro
Param(s): -
Returns: -
Purpose: Displays the second part of the introduction, where Shirley finds herself in the mirror world.
*/

void storyMirrorWorldIntro(void)
{
	printLine();

	printf("\"Where am I?\" Shirley wondered aloud.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("Was this the meadow that Eleanor believed she saw?\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("Regardless, she knew what she must do.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("She would find a way to leave this strange land.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("She promised to remain by Eleanor's side after all.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("A shuffle at Shirely's side alerted her.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Who are you?\" she asked the woman dressed in purple.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"I am what you may know as a witch, little lady.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("Her eyes narrowed. Witches were known to be vile creatures who performed harmful magic.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("But relying on this witch could be her only way of returning home.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Please Miss Witch, I am unable to find my way home.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Your concern is secondary to mine, little lady.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Please! My sister is ill--I have to return to her side!\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("The witch frowned contemplatively. \"It seems that you have a desperate wish, don't you?\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Very well, I will assist you if you help grant my wish. Do we have a deal?\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Yes.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"The Princess of Spades has decided to hold a competition,\" the witch explained.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Bet any amount you wish so long as it won't bankrupt the amount you are given.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"The important part is that anyone who wins %d consecutive hands of Black Jack will be given access to unlimited cake.\"\n", ROUNDS_TO_WIN_STORY);
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Cake?\" she gasped. \"You refused to help in favor of cake?\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Cake is the food of heavens. Any fool would know that,\" she scoffed.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"If I help you win all those rounds of Black Jack, you will send me home, right?\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"Of course,\" the witch smiled. \"I always fulfill my promises, unlike those pesky hypocrites calling themselves Knights of Justice.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"I will help you then,\" Shirley declared, deciding not to think too much of the witch's comment.\n");
	printf("\n");
	Sleep(FOUR_SEC);

	printf("\"You can do whatever you wish, but a win will only be a win if you gain more than you originally had after the game. You will lose as soon as you lose any amount you bet. Ending in a draw is a loss to me, as you must have consecutive wins.\"\n");
	printf("\n");
	Sleep(FOUR_SEC);

}

/*
-----------------------------------------------------------------
	Functions to Execute Story Mode
-----------------------------------------------------------------
*/

/*
Name: checkResultSMode
Param(s): remainingChips - the player chip total, previousChipAmount - the chip total before this round
Returns: -
Purpose: Determines if a player has won or lost the round (for story mode).
*/

int checkResultSMode(double remainingChips, double previousChipAmount)
{
	if(remainingChips > previousChipAmount)
	{
		return TRUE;
	}

	return FALSE;
}

/*
Name: executeGameinStoryMode
Param(s): -
Returns: WIN_STORY if the player has won the story version of the game, LOSE_STORY otherwise
Purpose: Runs the Black Jack game sequence and determines if the user has fulfilled the story conditions.
*/

int executeGameinStoryMode(void)
{
	//Declare tracking variables
	double chips;
	double bet;
	double previousChipAmount;
	int numWins;

	chips = NUM_CHIPS;
	previousChipAmount = chips;
	numWins = 0;

	do
	{
		bet = makeBet(chips);
		
		if(bet == EXIT)
		{
			return (int)bet;
		}

		chips = runGame(chips, bet);

		if(checkResultSMode(chips, previousChipAmount))
		{
			numWins++;
			printf("You have won %d game(s) thus far.\n", numWins);
			printf("\n");

		}
		
		else
		{
			 return LOSE_STORY;
		}

		previousChipAmount = chips;

	}while(numWins < ROUNDS_TO_WIN_STORY);

	return WIN_STORY;

}

/*
Name: processStoryResult
Param(s): gameResult - WIN_STORY if the game was won, LOSE_STORY otherwise
Returns: -
Purpose: Executes the appropriate story exerpt based on whether a user has won or lost the game's story mode.
*/


void processStoryResult(int gameResult)
{
	if(gameResult == WIN_STORY)
	{
		goodEnd();
	}
	
	else if(gameResult != EXIT)
	{
		badEnd();
	}
}

/*
Name: storyMode
Param(s): -
Returns: -
Purpose: Executes the game in story mode.
*/

void storyMode(void)
{
	int gameResult;

	storyIntroduction();
	storyMirrorWorldIntro();

	gameResult = executeGameinStoryMode();
	processStoryResult(gameResult);

}

/*
-----------------------------------------------------------------
	Miscellaneous Functions
-----------------------------------------------------------------
*/

/*
Name: initializeArray
Param(s): arrayToInit[] - array requiring initialization, maxIndex - the number of memory spaces allotted in the array, 
	      initval - the value to initialize the array with
Returns: arrayToInit[] - the array with the initialization value included in all memory slots.
Purpose: Allows the initialization of the deck, player hand, and dealer hand.
*/

void initializeArray(int arrayToInit[], int maxIndex, int initVal)
{
	int currentIndex = 0;

	while(currentIndex < maxIndex)
	{
		arrayToInit[currentIndex] = initVal;
		currentIndex++;
	}

}

/*
Name: printLine
Param(s): -
Returns: -
Purpose: Displays a dashed line of length OPENING_LINE_LENGTH.
*/

void printLine(void)
{
	int lineLength;
	
	lineLength= OPENING_LINE_LENGTH;
	while(lineLength > 0)
	{
		printf("-");
		lineLength--;
	}

	printf("\n");
	printf("\n");
}


/*
Name: runGame
Param(s): chips - the player chip total, bet - the bet made by the user
Returns: chipsRemaining - the number of chips remaining.
Purpose: Runs a single game of BlackJack
*/
double runGame (double chips, double bet)
{
	int numPlayerCards;
	int numDealerCards;
	int playerSum;
	int dealerSum;
	int blackJack;
	int insurance;
	double chipsRemaining; //Tracks the chips remaining after a win or loss.

	//Declare deck, and player and dealer hands
	int deck[NUM_CARDS]; //A bank for the 52 cards of a single deck to track which cards have been used.
	int player[MAX_HAND]; //Space for cards to fill the player's hand. Maximum number of cards is 11 based on 4 Aces, 4 2's, and 3 3's = 21.
	int dealer[MAX_HAND]; //Space for cards to fill the dealer's hand.

	//Initialize tracking variables
	numPlayerCards = EMPTY; //Begin with no cards for both player and dealer.
	numDealerCards = EMPTY;
	chipsRemaining = chips;
	blackJack = FALSE;

	//Initialize arrays
	initializeArray(deck, NUM_CARDS, MAX_NUM_PER_CARD); 
	initializeArray(player, MAX_HAND, EMPTY); 
	initializeArray(dealer, MAX_HAND, EMPTY);
	
	//Two cards are drawn for the dealer

	numDealerCards = openCard(deck, dealer);
	dealerSum = sumCards(dealer, numDealerCards);
	printDealerHidden(dealer);
	//Two cards are drawn for the player

	numPlayerCards = openCard(deck, player);
	playerSum = sumCards(player, numPlayerCards);
	
	printHandAndSum(player, numPlayerCards, 0, FALSE);

	if(checkBlackJack(playerSum, numPlayerCards))
	{
		return calculateWinForBlackJack(bet, chipsRemaining);

	}

	else{
		
		insurance = insuranceCheck(dealer);
		
		if(insurance)
		{
			chipsRemaining = chipsRemaining - 0.5*bet;
		}

		if(checkBlackJack(dealerSum, numDealerCards))
		{
			chipsRemaining = processInsurance(insurance, dealer, dealerSum, chipsRemaining, bet);
			blackJack = TRUE;
		}
	}
	
	chipsRemaining = playerAndDealerTurns(numPlayerCards, numDealerCards, playerSum, dealerSum, blackJack, insurance, chipsRemaining, deck, player, dealer, bet);
	
	return chipsRemaining;
}

/*
-----------------------------------------------------------------
	Main Body
-----------------------------------------------------------------
*/

int main (void)
{
	time_t t;
	srand((unsigned) time(&t)); //Initialize the random number generator
	

	mainMenu();

	
	return 0;
}