/*
 * Deck.h
 *
 *  Created on: Nov 6, 2017
 *      Author: mccampitelli
 */

#ifndef DECK_H_
#define DECK_H_

#include <vector>
#include <string>
#include <map>
using namespace std;

class Deck {

	struct Card {
		int rank;
		string suit;
		int handRank;
	};

	//this function is used to sort the player's hand in ascending order of Card rank.
	struct cmp {
		bool operator ()(const struct Card &a,
				const struct Card &b) const {
			return a.rank < b.rank;
		}
	};
public:
	Deck();
	vector<Card> vDeck;
	void shuffle();
	void print(map<string, vector<Card> > &game);
	void deal(vector<Deck::Card> &playerHand);
	void sortHand(vector<Card> &playerHand);
	int handValue(vector<Card> playerHand);
	bool flush(vector<Card> playerHand);
	bool straight(vector<Card> playerHand);
	bool straightFlush(vector<Card> playerHand);
	bool threeOfKind(vector<Card> playerHand);
	bool fourOfKind(vector<Card> playerHand);
	bool twoPair(vector<Card> playerHand);
	bool fullHouse(vector<Card> playerHand);
	bool pair(vector<Card> playerHand);
	int highCard(vector<Card> playerHand);
	bool royalFlush(vector<Card> playerHand);
	void reDraw(vector<Card> &playerHand, int cardNumber);
	void menu();
	void winner(map<string, vector<Deck::Card> > &game);
	int pairStrength(vector<Card> playerHand);
	int twoPairStrength(vector<Card> playerHand);
	int threeOfKindStrength(vector<Card> playerHand);
	int fourOfKindStrength(vector<Card> playerHand);
	int fullHouseStrength(vector<Card> playerHand);
	int cardRanks[13] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	string cardSuits[4] { "Hearts", "Spades", "Clubs", "Diamonds" };
};

#endif /* DECK_H_ */
