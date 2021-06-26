#pragma once
#include "Types.h"
struct Card {
	CULTURA cultura;
	MIEMBRO_FAMILIA familia;
	Card(const CULTURA _cultura, const MIEMBRO_FAMILIA _familia) {
		cultura = _cultura;
		familia = _familia;
	}

	static const std::string culturaToString(CULTURA _cultura) {
		switch (_cultura)
		{
		case CULTURA::ARABE:
			return "ARABE";
			break;
		case CULTURA::BANTU:
			return "BANTU";
			break;
		case CULTURA::CHINA:
			return "CHINA";
			break;
		case CULTURA::ESQUIMAL:
			return "ESQUIMAL";
			break;
		case CULTURA::INDIA:
			return "INDIA";
			break;
		case CULTURA::MEXICANA:
			return "MEXICANA";
			break;
		case CULTURA::TIROLESA:
			return "TIROLESA";
			break;
		default:
			break;
		}
		
	}

	static const std::string familiaToString(MIEMBRO_FAMILIA _familia) {
		switch (_familia)
		{
		case MIEMBRO_FAMILIA::ABUELO:
			return "ABUELO";
			break;
		case MIEMBRO_FAMILIA::ABUELA:
			return "ABUELA";
						break;
		case MIEMBRO_FAMILIA::PADRE:
			return "PADRE";
			break;
		case MIEMBRO_FAMILIA::MADRE:
			return "MADRE";
			break;
		case MIEMBRO_FAMILIA::HIJO:
			return "HIJO";
			break;
		case MIEMBRO_FAMILIA::HIJA:
			return "HIJA";
			break;
		default:
			break;
		}
	}
};