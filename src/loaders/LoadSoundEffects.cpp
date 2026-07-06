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
#include "headers/LoadSoundEffects.hpp"

namespace {
	struct SoundEffectData {
		const char* fileName;
	};

	constexpr auto SOUND_EFFECTS_LIST = std::to_array<SoundEffectData>({ // Création des bruitages : SoundEffectData(fileName)
		{"Blow1.wav"},                   // 0
		{"Kill1.wav"},                   // 1
		{"Damage3.ogg"}                  // 2
	});
}

/** Charge et retourne tous les bruitages **/
std::unordered_map<std::string, SoundEffect> LoadSoundEffects::get() {
	std::unordered_map<std::string, SoundEffect> soundEffects;
	soundEffects.reserve(SOUND_EFFECTS_LIST.size());

	for (const auto& data : SOUND_EFFECTS_LIST) {
		soundEffects.try_emplace(data.fileName, data.fileName);
	}

	return soundEffects;
}