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
#include <stdexcept>

#include "headers/LoadSkins.hpp"

namespace {
	struct SkinData {
		const char* fileName;
	};

	constexpr auto SKINS_LIST = std::to_array<SkinData>({ // Création des skins : SkinData(fileName)
		{"Evil.png"}       // 0
	});
}

/** Charge et retourne toutes les skins **/
std::unordered_map<std::string, Skin> LoadSkins::get(SDL_Renderer* renderer) {
	if (!renderer) throw std::invalid_argument("LoadSkins::get : renderer NULL");
	std::unordered_map<std::string, Skin> skins;
	skins.reserve(SKINS_LIST.size());

	for (const auto& data : SKINS_LIST) {
		skins.try_emplace(data.fileName, renderer, data.fileName);
	}
	return skins;
}