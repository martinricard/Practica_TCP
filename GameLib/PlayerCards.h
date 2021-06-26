#pragma once
#include "Card.h"
#include <vector>
#include <map>
struct PlayerCards {
	int actualCards;

	int actualTurn;
	std::vector<Card>playerCards;

	bool hasCardInPlayerCards(Card _card) {
	}

};