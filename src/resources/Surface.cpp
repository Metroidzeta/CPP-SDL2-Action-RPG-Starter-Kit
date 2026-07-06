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
#include <stdexcept>
#include <string>
#include <utility> // std::exchange
#include <format>

#include <SDL2/SDL_ttf.h>

#include "headers/Font.hpp"
#include "headers/Surface.hpp"

SDL_Surface* Surface::createTextSurface(const Font& font, std::string_view text, SDL_Color color) {
	if (text.empty()) throw std::invalid_argument("Texte vide");

	const std::string value(text);

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font.get(), value.c_str(), color);
	if (!surface) {
		throw std::runtime_error(std::format("Echec rendu texte : {} | TTF_GetError: {}", value, TTF_GetError()));
	}

	return surface;
}

SDL_Surface* Surface::createWrappedTextSurface(const Font& font, std::string_view text, SDL_Color color, int maxWidth) {
	if (text.empty()) throw std::invalid_argument("Texte vide");
	if (maxWidth <= 0) throw std::invalid_argument("maxWidth <= 0");

	const std::string value(text);

	SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(font.get(), value.c_str(), color, static_cast<Uint32>(maxWidth));
	if (!surface) {
		throw std::runtime_error(std::format("Echec rendu texte wrapped : {} | TTF_GetError: {}", value, TTF_GetError()));
	}

	return surface;
}

Surface::Surface(const Font& font, std::string_view text, SDL_Color color)
	: surface_(createTextSurface(font, text, color)) {}

Surface::Surface(const Font& font, std::string_view text, SDL_Color color, int maxWidth)
	: surface_(createWrappedTextSurface(font, text, color, maxWidth)) {}

/** Destructeur **/
Surface::~Surface() noexcept { SDL_FreeSurface(surface_); }

Surface::Surface(Surface&& other) noexcept
	: surface_(std::exchange(other.surface_, nullptr)) {}

Surface& Surface::operator=(Surface&& other) noexcept {
	if (this != &other) {
		SDL_FreeSurface(surface_);
		surface_ = std::exchange(other.surface_, nullptr);
	}

	return *this;
}