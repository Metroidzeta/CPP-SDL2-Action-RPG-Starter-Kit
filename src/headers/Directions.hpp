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

enum class Directions : int { DOWN = 0, LEFT, RIGHT, UP };

inline constexpr std::string_view toString(Directions d) noexcept {
	switch (d) {
		case Directions::DOWN:  return "DOWN";
		case Directions::LEFT:  return "LEFT";
		case Directions::RIGHT: return "RIGHT";
		case Directions::UP:    return "UP";
	}
	return "UNKNOWN";
}

inline std::ostream& operator<<(std::ostream& os, Directions d) {
	return os << toString(d);
}