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
#include <array>
#include <algorithm>
#include <cmath>

#include "headers/LoadFonts.hpp"
#include "../headers/Config.hpp"

namespace {
	struct FontData {
		const char* name;
		const char* fileName;
		int size;
	};

	constexpr auto FONTS_LIST = std::to_array<FontData>({ // Création des fonts : FontData(name, fileName, size)
		{"FPS", "arial.ttf", static_cast<int>(Config::CELL_SIZE * 0.34)},                        // 0 (FPS)
		{"Base", "arial.ttf", static_cast<int>(Config::CELL_SIZE * 0.68)},                       // 1 (Texte de base)
		{"Damage", "Zelda Breath of the Wild.otf", static_cast<int>(Config::CELL_SIZE * 0.95)}  // 2 (Coups d'attaques)
	});

	int scaleFontSize(int size, float scale) {
		if (scale <= 0.0f) scale = 1.0f;
		return std::max(1, static_cast<int>(std::lround(size * scale)));
	}
}

/** Charge et retourne toutes les fonts **/
std::unordered_map<std::string, Font> LoadFonts::get(float scale) {
	std::unordered_map<std::string, Font> fonts;
	fonts.reserve(FONTS_LIST.size());

	for (const auto& data : FONTS_LIST) {
		fonts.try_emplace(data.name, data.fileName, scaleFontSize(data.size, scale));
	}

	return fonts;
}