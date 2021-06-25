#pragma once
#include <vector>
#include "Card.h"

struct Deck {
	std::vector<Card*>deck;
	Deck() {
		deck.reserve(NUM_CULTURA * NUM_FAMILIA);
		for (int i = 0;i < NUM_CULTURA;i++) {
			for (int j = 0;j < NUM_FAMILIA;j++) {
				deck.push_back(new Card((CULTURA)i, (MIEMBRO_FAMILIA)j));
			}
		}
	}
};