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

#include "headers/LoadMusics.hpp"

namespace {
	struct MusicData {
		const char* fileName;
	};

	constexpr auto MUSICS_LIST = std::to_array<MusicData>({ // Création des musiques : MusicData(fileName)
		{"Castle_1.ogg"},                        // 0
		{"Sarosa.ogg"},                          // 1
		{"bahamut_lagoon.ogg"},                  // 2
		{"Castle_3.ogg"},                        // 3
		{"2000_ordeal.ogg"},                     // 4
		{"cc_viper_manor.ogg"},                  // 5
		{"suikoden-ii-two-rivers.ogg"},          // 6
		{"mystery3.ogg"},                        // 7
		{"hunter.ogg"},                          // 8
		{"illusionary_world.ogg"},               // 9
		{"chapt1medfill.ogg"}                    // 10
	});
}

/** Charge et retourne toutes les musiques **/
std::unordered_map<std::string, Music> LoadMusics::get() {
	std::unordered_map<std::string, Music> musics;
	musics.reserve(MUSICS_LIST.size());

	for (const auto& data : MUSICS_LIST) {
		musics.try_emplace(data.fileName, data.fileName);
	}

	return musics;
}