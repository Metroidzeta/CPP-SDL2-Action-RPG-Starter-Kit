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

#include <string>
#include <algorithm>
#include <cctype>
#include <array>

namespace Util {
	inline constexpr std::array<int, 11> VB {112, 57, 15, 232, 214, 177, 147, 108, 80, 32, 18};
	constexpr int keyForIndex(int i) noexcept { return (i * 31 + 7) & 0xFF;}

	/** Retourne true si la chaîne est vide OU uniquement composée d'espaces blancs */
	inline bool isBlank(const std::string& s) {
		return std::all_of(s.begin(), s.end(),
						   [](unsigned char c){ return std::isspace(c); });
	}

	inline std::string u32_to_u8(const std::u32string& u32) {
		std::string out;
		out.reserve(u32.size());

		for (char32_t c : u32) {
			if (c <= 0x7F) {
				out.push_back(static_cast<char>(c));
			}
			else if (c <= 0x7FF) {
				out.push_back(static_cast<char>(0xC0 | (c >> 6)));
				out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
			}
			else if (c <= 0xFFFF) {
				out.push_back(static_cast<char>(0xE0 | (c >> 12)));
				out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
			}
			else {
				out.push_back(static_cast<char>(0xF0 | (c >> 18)));
				out.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
				out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
			}
		}
		return out;
	}

	inline std::string trim(std::string s) {
		auto notSpace = [](unsigned char c) { return !std::isspace(c); };

		s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
		s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());

		return s;
	}
}