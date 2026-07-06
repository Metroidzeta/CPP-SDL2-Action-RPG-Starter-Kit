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

#include "../../resources/headers/Music.hpp"

/**
 * Construit et retourne la map complète (nom -> musique) des musiques du jeu.
 */
class LoadMusics final {
private:
	LoadMusics() = delete; // Empêche toute instanciation

public:
	/** Charge et retourne toutes les musiques **/
	static std::unordered_map<std::string, Music> get();
};