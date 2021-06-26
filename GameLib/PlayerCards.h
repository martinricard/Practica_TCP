#pragma once
#include "Card.h"
#include <vector>
#include <map>
struct PlayerCards {
	int actualCards;
	int isPlaying;
	int actualTurn;
	int numOfCards;
	std::vector<Card>playerCards;

	bool hasCardInPlayerCards(Card _card) {
	}
	void PrintHand() {
		int i = 0;
		for (auto it : playerCards) {
			std::cout << "Carta "<<i<<": "<< Card::culturaToString(it.cultura)<<"     "<< Card::familiaToString(it.familia)<<std::endl;
			i++;
		}
	}

	void giveCard(Card _card) {
		numOfCards++;
		playerCards.push_back(_card);
	}

	bool checkCard(Card _card) {
		for (auto it : playerCards) {
			if (it.cultura==_card.cultura&&it.familia==_card.familia) {
				return true;
			}
		}
		return false;
	}

	void eraseCard(Card _card) {

		for (auto i = 0; i < playerCards.size(); i++) {

			if (playerCards[i].cultura == _card.cultura && playerCards[i].familia == _card.familia) {

					std::vector<Card>::iterator it = playerCards.begin() + i;

					playerCards.erase(it);
				}


			}
		numOfCards--;
		if (numOfCards <= 0)
		{
			isPlaying = false;
		}

	}
};