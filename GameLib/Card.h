#pragma once
#include "Types.h"
struct Card {
	CULTURA cultura;
	MIEMBRO_FAMILIA familia;
	Card(const CULTURA _cultura, const MIEMBRO_FAMILIA _familia) {
		cultura = _cultura;
		familia = _familia;
	}
};