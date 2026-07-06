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

#include <string_view>
#include <SDL2/SDL.h>

class Font;

class Surface final {
private:
	SDL_Surface* surface_ = nullptr;

	/** Méthodes privées **/
	static SDL_Surface* createTextSurface(const Font& font, std::string_view text, SDL_Color color);
	static SDL_Surface* createWrappedTextSurface(const Font& font, std::string_view text, SDL_Color color, int maxWidth);

public:
	/** Constructeurs **/
	Surface(const Font& font, std::string_view text, SDL_Color color);
	Surface(const Font& font, std::string_view text, SDL_Color color, int maxWidth);

	/** Destructeur **/
	~Surface() noexcept;

	/** Copie interdite **/
	Surface(const Surface&) = delete;
	Surface& operator=(const Surface&) = delete;

	/** Move autorisé **/
	Surface(Surface&& other) noexcept;
	Surface& operator=(Surface&& other) noexcept;

	/** Getters **/
	SDL_Surface* get() const noexcept { return surface_; }

	int getWidth() const noexcept { return surface_ ? surface_->w : 0; }
	int getHeight() const noexcept { return surface_ ? surface_->h : 0; }
};