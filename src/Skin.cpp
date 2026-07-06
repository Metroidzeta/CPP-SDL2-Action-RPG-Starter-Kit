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
#include <functional> // std::hash
#include <stdexcept>

#include "headers/Config.hpp"
#include "headers/Util.hpp"
#include "headers/Skin.hpp"

namespace {
	std::string validateFileName(const std::string& fileName) {
		if (fileName.empty() || Util::isBlank(fileName)) {
			throw std::invalid_argument("Nom du fichier de la skin vide");
		}
		return fileName;
	}
}

/** Constructeur **/
Skin::Skin(SDL_Renderer* renderer, const std::string& fileName)
	: name_(validateFileName(fileName)), texture_(renderer, std::string(Config::SKINS_DIRECTORY) + '/' + fileName) {
	const int minWidth = COLS * REGION_WIDTH;
	const int minHeight = ROWS * REGION_HEIGHT;

	// On récupère la taille réelle de l'image
	int width = 0, height = 0;
	texture_.querySize(width, height);
	if (width < minWidth || height < minHeight) { 
		throw std::runtime_error(
			"Dimensions de skin invalides : " + std::to_string(width) + "x" + std::to_string(height)
			+ " (minimum " + std::to_string(minWidth) + "x" + std::to_string(minHeight) + " pixels)"
		);
	}

	for (int i = 0; i < ROWS; ++i) {
		const int y = i * REGION_HEIGHT;
		const int lineIndex = i * COLS;
		for (int j = 0; j < COLS; ++j) {
			regions_[lineIndex + j] = { j * REGION_WIDTH, y, REGION_WIDTH, REGION_HEIGHT };
		}
	}
}

/** Affichage **/
void Skin::draw(SDL_Renderer* renderer, int numRegion, int x, int y, int size) const {
	if (!renderer) throw std::invalid_argument("Renderer NULL");
	if (numRegion < 0 || numRegion >= TOTAL_REGIONS) throw std::out_of_range("Indice region invalide: " + std::to_string(numRegion));

	SDL_Rect dst{ x, y, size, size };
	if (SDL_RenderCopy(renderer, texture_.get(), &regions_[numRegion], &dst) != 0) {
		throw std::runtime_error(std::string("Echec SDL_RenderCopy: ") + SDL_GetError());
	}
}

/** Operator== et hash() **/
bool Skin::operator==(const Skin& other) const noexcept {
	return name_ == other.name_
		&& texture_.get() == other.texture_.get();
}

std::size_t Skin::hash() const noexcept {
	return std::hash<std::string>()(name_)
		^ (std::hash<void*>()(texture_.get()) << 1);
}