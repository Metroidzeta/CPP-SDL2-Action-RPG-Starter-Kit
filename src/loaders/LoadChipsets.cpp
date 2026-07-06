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

#include "headers/LoadChipsets.hpp"

namespace {
	struct ChipsetData {
		const char* fileName;
		int tileSize;
	};

	/** Liste des chipsets **/
	constexpr auto CHIPSETS_LIST = std::to_array<ChipsetData>({ // Création des chipsets : ChipsetData(fileName, tileSize [par défaut : 16])
		{"BZ.png",                  16},       // 0
		{"VillageTangaFinal.png",   16},       // 1
		{"grey_cas42.png",          16},       // 2
		{"PalaisRoland2.png",       16},       // 3
		{"PalaisRolandInt.png",     16},       // 4
		{"PalaisRolandNouveau.png", 48},       // 5
		{"marais2.png",             16},       // 7
		{"MaraisTanga.png",         16},       // 6
		{"Coacville_exterieur.png", 16},       // 8
		{"chipset173.png",          16},       // 9
		{"chipset175.png",          16},       // 10
		{"HunterArene.png",         16},       // 11
		{"grass.png",               32},       // 12
		{"chipset5c.png",           16}        // 13
	});
}

/** Charge et retourne tous les chipsets **/
std::unordered_map<std::string, Chipset> LoadChipsets::get(SDL_Renderer* renderer) {
	if (!renderer) throw std::invalid_argument("LoadChipsets::get : renderer NULL");
	std::unordered_map<std::string, Chipset> chipsets;
	chipsets.reserve(CHIPSETS_LIST.size());

	for (const auto& data : CHIPSETS_LIST) {
		chipsets.try_emplace(data.fileName, renderer, data.fileName, data.tileSize);
	}

	return chipsets;
}