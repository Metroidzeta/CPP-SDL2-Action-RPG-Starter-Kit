/**
 * @author Alain Barbier alias "Metroidzeta"
 * Copyright © 2026 Alain Barbier (Metroidzeta) - All rights reserved.
 *
 * This file is part of the project covered by the
 * "Educational and Personal Use License / Licence d’Utilisation Personnelle et Éducative".
 *
 * Permission is granted to fork and use this code for educational and personal purposes only.
 *
 * Commercial use, redistribution, or public republishing of modified versions
 * is strictly prohibited without the express written consent of the author.
 *
 * Coded with SDL2 (Simple DirectMedia Layer 2).
 */
#pragma once
#include <string_view>
#include <ostream>

enum class HeroClass : int { BARBARIAN = 0, WARRIOR, ROGUE };

inline std::string_view toString(HeroClass c) {
	switch (c) {
		case HeroClass::BARBARIAN: return "Barbare";
		case HeroClass::WARRIOR:   return "Guerrier";
		case HeroClass::ROGUE:     return "Voleur";
	}
	return "INCONNU";
}

inline std::ostream& operator<<(std::ostream& os, HeroClass c) {
	return os << toString(c);
}