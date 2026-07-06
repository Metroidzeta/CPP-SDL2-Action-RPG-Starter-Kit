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
#include <cstddef>
#include <array>
#include <SDL2/SDL.h>

#include "../resources/headers/Texture.hpp"

class Skin final {
private:
	static constexpr int REGION_WIDTH  = 48;
	static constexpr int REGION_HEIGHT = 48;
	static constexpr int ROWS = 4;
	static constexpr int COLS = 3;
	static constexpr int TOTAL_REGIONS = ROWS * COLS;

	const std::string name_;
	Texture texture_;
	std::array<SDL_Rect, TOTAL_REGIONS> regions_{};

public:
	/** Constructeurs **/
	Skin() = delete; // vide interdit
	Skin(SDL_Renderer* renderer, const std::string& fileName);

	/** Getters **/
	const std::string& getName() const noexcept { return name_; }

	/** Méthodes utilitaires **/
	void draw(SDL_Renderer* renderer, int numRegion, int x, int y, int size) const;

	/** Operator ==, != et hash() **/
	bool operator==(const Skin& other) const noexcept;
	bool operator!=(const Skin& other) const noexcept { return !(*this == other); }
	std::size_t hash() const noexcept;
};