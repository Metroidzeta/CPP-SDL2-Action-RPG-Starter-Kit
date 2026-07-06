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
#include <vector>
#include <SDL2/SDL.h>
#include "../resources/headers/Texture.hpp"

/**
 * Représente un chipset, c’est-à-dire un ensemble de tuiles graphiques
 * découpées à partir d’une image source unique servant à composer les cartes du jeu.
 */
class Chipset final {
private:
	std::string name_;
	Texture texture_;
	int tileSize_ = 0;
	int tileCountWidth_ = 0, tileCountHeight_ = 0;
	std::vector<SDL_Rect> tiles_;

public:
	static constexpr int EMPTY_TILE = 0; // par défaut : 0

	/** Constructeurs **/
	Chipset() = delete; // vide interdit
	Chipset(SDL_Renderer* renderer, const std::string& fileName, int tileSize);

	/** Getters **/
	const std::string& getName() const noexcept { return name_; }
	const Texture& getTexture() const noexcept { return texture_; }
	const SDL_Rect& getTile(int index) const;
	int getTileSize() const noexcept { return tileSize_; }
	int getTileCountWidth() const noexcept { return tileCountWidth_; }
	int getTileCountHeight() const noexcept { return tileCountHeight_; }
};