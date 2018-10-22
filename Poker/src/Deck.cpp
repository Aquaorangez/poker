/*
 * Deck.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: mccampitelli
 */

#include "Deck.h"
#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
using namespace std;

const int NUMBER_OF_CARD_RANKS = 13;
const int NUMBER_OF_SUITS = 4;

Deck::Deck() {

	//create a deck with all 52 unique cards
	for (int i = 0; i < NUMBER_OF_CARD_RANKS; i++) {
		for (int j = 0; j < NUMBER_OF_SUITS; j++) {
			struct Card card = { cardRanks[i], cardSuits[j] };
			vDeck.push_back(card);
		}
	}
}

void Deck::shuffle() {

	//shuffles the deck by swapping each card with another random card in the deck
	srand(time(NULL));
	int r;
	for (int i = 0; i < 52; i++) {
		r = rand() % 52;
		Card temp = vDeck.at(r);
		vDeck.at(r) = vDeck.at(i);
		vDeck.at(i) = temp;
	}
}

void Deck::print(map<string, vector<Deck::Card> > &game) {

	//loops through each player's hand and prints each individual card
	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		cout << it->first << "'s Hand:" << endl;

		for (vector<Card>::iterator it2 = it->second.begin();
				it2 != it->second.end(); ++it2) {
			//switches the face value cards from integer to string i.e. 11 = "J". Easier to work with integer array.
			if (it2->rank == 11) {
				cout << "J of " << it2->suit << endl;
			} else if (it2->rank == 12) {
				cout << "Q of " << it2->suit << endl;
			} else if (it2->rank == 13) {
				cout << "K of " << it2->suit << endl;
			} else if (it2->rank == 14) {
				cout << "A of " << it2->suit << endl;
			} else
				cout << it2->rank << " of " << it2->suit << endl;

		}

		//Checks the value of hand and prints whats it is. i.e. pair, two pair..
		cout << it->first << " has ";
		it->second[4].handRank = handValue(it->second);
		cout << endl;
	}

	//asks if the user wants to exchange any of his cards, up to a maximum of 4.
	//The user selects the cards and they are exchanged with the remaining cards in the deck.

	int numCardsToExchange = 0, cardToExchange = 0;
	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		cout << it->first << ": How many cards would like to redraw? (0 to 4)"
				<< endl;
		cin >> numCardsToExchange;

		if (numCardsToExchange == 0) {
			continue;
		}

		for (int i = 0; i < numCardsToExchange; i++) {
			cout
					<< "Select card to exchange. \"1\" = first card, \"2\" = second card etc. Scroll up to see your hand."
					<< endl;
			cin >> cardToExchange;
			reDraw(it->second, cardToExchange);
		}

	}

	//this reorganizes the players cards after the exchange, in ascending order.
	//Makes calculating the value of the hand easier.
	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		sortHand(it->second);
	}
	//prints the cards again, like above.
	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		cout << it->first << "'s Hand:" << endl;

		for (vector<Card>::iterator it2 = it->second.begin();
				it2 != it->second.end(); ++it2) {
			if (it2->rank == 11) {
				cout << "J of " << it2->suit << endl;
			} else if (it2->rank == 12) {
				cout << "Q of " << it2->suit << endl;
			} else if (it2->rank == 13) {
				cout << "K of " << it2->suit << endl;
			} else if (it2->rank == 14) {
				cout << "A of " << it2->suit << endl;
			} else
				cout << it2->rank << " of " << it2->suit << endl;

		}
		cout << it->first << " has ";
		it->second[4].handRank = handValue(it->second);
		cout << endl;
	}

}
void Deck::menu() {
	char start;
	int players;
	string playerName;

	//prompts the user for the amount of players and their names.
	cout << "Ready to play 5-card draw? (y or n) ";
	cin >> start;
	if (start == 'n' || cin.fail()) {
		cout << "Maybe next time!";
		exit(1);
	}

	cout << "How many players are playing? ";
	cin >> players;

	map<string, vector<Card> > game;
	vector<Card> hand;

	for (int i = 1; i <= players; i++) {
		cout << "Enter player name: ";
		cin >> playerName;
		game[playerName];
	}

	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		for (int j = 0; j < 5; j++) {
			deal(it->second);
		} //	if (flush(playerHand) == true && straight(playerHand) == true) {
		  //
	}
	//sorts the initial hand of 5 cards in ascending order
	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		sortHand(it->second);
	}

	print(game);
	winner(game);

}

//deals a single card to each players hand and removes that card from the deck.
void Deck::deal(vector<Card> &playerHand) {
	Card card = vDeck.back();
	playerHand.push_back(card);
	vDeck.pop_back();

}
//sorts the players hand in ascending order
void Deck::sortHand(vector<Card> &playerHand) {
	sort(playerHand.begin(), playerHand.end(), cmp());

}

//calculates the value of the players hand using boolean helper functions below.
int Deck::handValue(vector<Card> playerHand) {

	if (royalFlush(playerHand)) {
		cout << "a Royal Flush!\n";
		return 10;
	}
	if (straightFlush(playerHand)) {
		cout << "a Straight Flush!\n";
		return 9;
	}
	if (fullHouse(playerHand)) {
		cout << "a Full House!\n";
		return 7;
	}
	if (fourOfKind(playerHand)) {
		cout << "Four of Kind!\n";
		return 8;
	}
	if (flush(playerHand)) {
		cout << "a Flush!\n";
		return 6;
	}
	if (straight(playerHand)) {
		cout << "a Straight!\n";
		return 5;
	}
	if (twoPair(playerHand)) {
		cout << "Two Pair!\n";
		return 3;
	}
	if (threeOfKind(playerHand)) {
		cout << "Three of a Kind!\n";
		return 4;
	}
	if (pair(playerHand)) {
		cout << "a Pair!\n";
		return 2;
	}
	cout << "High Card.\n";
	return 1;

}

bool Deck::flush(vector<Card> playerHand) {
	int count = 0;

	for (vector<Card>::iterator it2 = playerHand.begin();
			it2 != playerHand.end() - 1; ++it2) {
		if (it2->suit == (it2 + 1)->suit) {
			count++;
		}
	}
	if (count == 4) {
		return true;
	}
	return false;
}

bool Deck::straight(vector<Card> playerHand) {
	int count = 0;

	for (vector<Card>::iterator it2 = playerHand.begin();
			it2 != playerHand.end(); ++it2) {
		if (it2->rank == ((it2 + 1)->rank) + 1) {
			count++;
		}
	}
	if (count == 4) {
		return true;
	}
	return false;
}
bool Deck::straightFlush(vector<Card> playerHand) {
	if (flush(playerHand) == true && straight(playerHand) == true) {
		return true;
	}
	return false;
}
bool Deck::threeOfKind(vector<Card> playerHand) {
	int count = 0;

	for (vector<Card>::iterator it2 = playerHand.begin();
			it2 != playerHand.end() - 1; ++it2) {
		if (it2->rank == (it2 + 1)->rank) {
			count++;
		}
	}
	if (count == 2) {
		return true;
	}
	return false;
}
bool Deck::fourOfKind(vector<Card> playerHand) {
	int count = 0;

	for (vector<Card>::iterator it2 = playerHand.begin();
			it2 != playerHand.end() - 1; ++it2) {
		if (it2->rank == (it2 + 1)->rank) {
			count++;
		}
	}
	if (count == 3) {
		return true;
	}
	return false;
}

bool Deck::twoPair(vector<Card> playerHand) {
	if (threeOfKind(playerHand)) {
		if (playerHand[0].rank != playerHand[2].rank
				&& playerHand[1].rank != playerHand[3].rank
				&& playerHand[2].rank != playerHand[5].rank)
			return true;
	}
	return false;
}
bool Deck::fullHouse(vector<Card> playerHand) {
	if (fourOfKind(playerHand)) {
		if ((playerHand[0].rank == playerHand[1].rank
				&& playerHand[2].rank == playerHand[3].rank) || (playerHand[1].rank == playerHand[2].rank
						&& playerHand[3].rank == playerHand[4].rank) ) {
			return true;
		}
	}
	return false;
}
bool Deck::pair(vector<Card> playerHand) {
	int count = 0;

	for (vector<Card>::iterator it2 = playerHand.begin();
			it2 != playerHand.end() - 1; ++it2) {
		if (it2->rank == (it2 + 1)->rank) {
			count++;
		}
	}
	if (count == 1) {
		return true;
	}
	return false;
}

int Deck::highCard(vector<Card> playerHand) {
	return playerHand[4].rank;
}

bool Deck::royalFlush(vector<Card> playerHand) {
	if (straight(playerHand) && flush(playerHand)) {
		if (playerHand[4].rank == 14) {
			return true;
		}
	}
	return false;
}

//this function removes a specified card and deals another random card to them. removes that card from the deck
void Deck::reDraw(vector<Card> &playerHand, int cardNumber) {
	playerHand.erase(playerHand.begin() + cardNumber - 1);
	Card card = vDeck.back();
	playerHand.insert(playerHand.begin() + cardNumber - 1, card);
	vDeck.pop_back();

}

//this calculates and displays the winner, making sure to calculate the right winner
//even if they have the same value hand by checking the high card or value of the highest pair etc.
void Deck::winner(map<string, vector<Deck::Card> > &game) {
	int winningHand = 0, winningHandStr = 0;

	//checks the winning hand value. I.e. a pair, three of a kind etc
	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		if (it->second[4].handRank > winningHand) {
			winningHand = it->second[4].handRank;
		}
	}
	//checks each individuals hand if it equals the winning hand. If 2 or more hands are equal to the winning hand value
	//checks each hand for high card, highest pair, highest three of a kind etc.
	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		if (it->second[4].handRank == winningHand) {
			if (winningHand == 1 || winningHand == 5 || winningHand == 6) {
				if (highCard(it->second) > winningHandStr) {
					winningHandStr = highCard(it->second);
					it->second[4].handRank = winningHandStr;
				}
			} else if (winningHand == 2) {
				if (pairStrength(it->second) > winningHandStr) {
					winningHandStr = pairStrength(it->second);
					it->second[4].handRank = winningHandStr;
				}
			} else if (winningHand == 3) {
				if (twoPairStrength(it->second) > winningHandStr) {
					winningHandStr = twoPairStrength(it->second);
					it->second[4].handRank = winningHandStr;
				}
			} else if (winningHand == 4) {
				if (threeOfKindStrength(it->second) > winningHandStr) {
					winningHandStr = threeOfKindStrength(it->second);
					it->second[4].handRank = winningHandStr;
				}
			} else if (winningHand == 8) {
				if (fourOfKindStrength(it->second) > winningHandStr) {
					winningHandStr = fourOfKindStrength(it->second);
					it->second[4].handRank = winningHandStr;
				}
			} else if (winningHand == 7) {
				if (fullHouseStrength(it->second) > winningHandStr) {
					winningHandStr = fullHouseStrength(it->second);
					it->second[4].handRank = winningHandStr;
				}
			}
		}
	}

	//prints the winner!
	for (map<string, vector<Card> >::iterator it = game.begin();
			it != game.end(); ++it) {
		if (it->second[4].handRank == winningHandStr) {
			cout << it->first << " wins!!";
		}
	}
}

//these final helper functions check the strength of each individual hand in the case where 2 or more hands are equal

int Deck::pairStrength(vector<Card> playerHand) {
	int pairValue = 0;

	for (vector<Card>::iterator it2 = playerHand.begin();
			it2 != playerHand.end() - 1; ++it2) {
		if (it2->rank == (it2 + 1)->rank) {
			pairValue = it2->rank;
			return pairValue;

		}
	}
	return pairValue;
}

int Deck::twoPairStrength(vector<Card> playerHand) {
	int highPair = playerHand[4].rank;
	return highPair;
}

int Deck::threeOfKindStrength(vector<Card> playerHand) {
	int threeValue = 0;

	if (playerHand[0].rank == playerHand[1].rank
			&& playerHand[0].rank == playerHand[2].rank) {
		threeValue = playerHand[0].rank;
	} else if (playerHand[1].rank == playerHand[2].rank
			&& playerHand[1].rank == playerHand[3].rank) {
		threeValue = playerHand[1].rank;
	} else
		threeValue = playerHand[4].rank;

	return threeValue;
}

int Deck::fourOfKindStrength(vector<Card> playerHand) {
	int fourValue = 0;

	if (playerHand[0].rank == playerHand[1].rank) {
		fourValue = playerHand[0].rank;
	} else
		fourValue = playerHand[1].rank;

	return fourValue;
}

int Deck::fullHouseStrength(vector<Card> playerHand) {
	int fullHouseValue = 0;

	if (playerHand[0].rank == playerHand[2].rank) {
		fullHouseValue = playerHand[0].rank;
	} else
		fullHouseValue = playerHand[4].rank;

	return fullHouseValue;
}

