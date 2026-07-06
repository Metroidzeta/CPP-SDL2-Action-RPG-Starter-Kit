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
#include <utility> // pour std::exchange
#include <format>
#include <SDL2/SDL_image.h>

#include "../headers/Util.hpp"
#include "headers/Surface.hpp"
#include "headers/Texture.hpp"

/** Constructeurs **/
Texture::Texture(SDL_Renderer* renderer, const std::string& path) {
	if (!renderer) throw std::invalid_argument("Renderer NULL");
	if (path.empty() || Util::isBlank(path)) throw std::invalid_argument("Chemin fichier de la texture vide");
	tex_ = IMG_LoadTexture(renderer, path.c_str());
	if (!tex_) {
		throw std::runtime_error(std::format("Echec chargement texture depuis image : {} | IMG_GetError : {}", path, IMG_GetError()));
	}
	if (SDL_SetTextureBlendMode(tex_, SDL_BLENDMODE_BLEND) != 0) {
		throw std::runtime_error(std::format("Echec SDL_SetTextureBlendMode : {}", SDL_GetError()));
	}
	if (SDL_SetTextureScaleMode(tex_, SDL_ScaleModeNearest) != 0) {
		throw std::runtime_error(std::format("Echec SDL_SetTextureScaleMode : {}", SDL_GetError()));
	}
}

Texture::Texture(SDL_Renderer* renderer, const Surface& surface) {
	if (!renderer) throw std::invalid_argument("Renderer NULL");
	tex_ = SDL_CreateTextureFromSurface(renderer, surface.get());
	if (!tex_) {
		throw std::runtime_error(std::format("Echec creation texture depuis surface | SDL_GetError: {}", SDL_GetError()));
	}
	if (SDL_SetTextureBlendMode(tex_, SDL_BLENDMODE_BLEND) != 0) {
		throw std::runtime_error(std::format("Echec SDL_SetTextureBlendMode : {}", SDL_GetError()));
	}
}

/** Destructeur **/
Texture::~Texture() noexcept { SDL_DestroyTexture(tex_); }

void Texture::querySize(int& width, int& height) const {
	if (!tex_) throw std::runtime_error("Texture::querySize() appelée sur une texture NULL");
	if (SDL_QueryTexture(tex_, nullptr, nullptr, &width, &height) != 0) {
		throw std::runtime_error(std::format("Echec SDL_QueryTexture : {}", SDL_GetError()));
	}
}

/** Move constructor **/
Texture::Texture(Texture&& other) noexcept
	: tex_(std::exchange(other.tex_, nullptr)) {}

/** Move assignment **/
Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		SDL_DestroyTexture(tex_); // Libère ancienne ressource
		tex_ = std::exchange(other.tex_, nullptr); // Move de la texture
	}
	return *this;
}