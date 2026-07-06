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
#include <unordered_map>
#include <SDL2/SDL.h>

#include "../../headers/Chipset.hpp"

/**
 * Construit et retourne la map complète (nom -> chipset) des chipsets du jeu.
 */
class LoadChipsets final {
private:
	LoadChipsets() = delete; // Empêche toute instanciation

public:
	/** Charge et retourne tous les chipsets **/
	static std::unordered_map<std::string, Chipset> get(SDL_Renderer* renderer);
};