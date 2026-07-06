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

#include "../headers/Config.hpp"
#include "headers/LoadUiAssets.hpp"

namespace {
	struct UiAssetData {
		const char* name;
		const char* fileName;
	};

	/** Liste des affichages **/
	constexpr auto UI_ASSETS_LIST = std::to_array<UiAssetData>({ // Création des affichages : new UiAssetData(name, fileName)
		{"Fioles", "fioles.png"},              // 0 (fioles)
		{"BarreXp", "xp.png"}                  // 1 (barre XP)
	});
}

/** Charge et retourne tous les affichages **/
std::unordered_map<std::string, Texture> LoadUiAssets::get(SDL_Renderer* renderer) {
	if (!renderer) throw std::invalid_argument("LoadUiAssets::get : renderer NULL");
	std::unordered_map<std::string, Texture> uiAssets;
	uiAssets.reserve(UI_ASSETS_LIST.size());

	for (const auto& data : UI_ASSETS_LIST) {
		const std::string path = std::string(Config::UI_ASSETS_DIRECTORY) + '/' + data.fileName;
		uiAssets.try_emplace(data.name, renderer, path);
	}

	return uiAssets;
}