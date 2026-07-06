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
#include <limits> //std::numeric_limits

#include "headers/Config.hpp"
#include "headers/Util.hpp"
#include "headers/Chipset.hpp"

static std::string requireValidFileName(const std::string& fileName) {
	if (fileName.empty() || Util::isBlank(fileName)) throw std::invalid_argument("Nom fichier du chipset vide");
	return fileName;
}

static std::vector<SDL_Rect> extractTiles(int tileSize, int tileCountWidth, int tileCountHeight) {
	const int nbTiles = tileCountWidth * tileCountHeight;
	std::vector<SDL_Rect> tiles;
	tiles.reserve(nbTiles);

	for (int i = 0; i < tileCountHeight; ++i) {
		const int y = i * tileSize;
		for (int j = 0; j < tileCountWidth; ++j) {
			tiles.push_back({ j * tileSize, y, tileSize, tileSize });
		}
	}

	return tiles;
}

/** Constructeur **/
Chipset::Chipset(SDL_Renderer* renderer, const std::string& fileName, int tileSize)
	: name_(requireValidFileName(fileName)), texture_(renderer, std::string(Config::CHIPSETS_DIRECTORY) + '/' + name_),
	tileSize_(tileSize) {
	if (tileSize_ < 1) throw std::invalid_argument("TileSize du chipset " + name_ + " < 1");

	int textureWidth = 0, textureHeight = 0;
	texture_.querySize(textureWidth, textureHeight);

	if (textureWidth % tileSize_ != 0 || textureHeight % tileSize_ != 0) {
		throw std::runtime_error(
			"Dimensions de l'image incompatibles avec la taille de tuile : "
			+ std::to_string(textureWidth) + "x" + std::to_string(textureHeight)
			+ " pour une taille de tuile de " + std::to_string(tileSize_) + " pixels"
		);
	}

	tileCountWidth_ = textureWidth / tileSize_;
	tileCountHeight_ = textureHeight / tileSize_;
	if (tileCountWidth_ > std::numeric_limits<int>::max() / tileCountHeight_) throw std::runtime_error("Trop de tuiles (overflow)");
	tiles_ = extractTiles(tileSize_, tileCountWidth_, tileCountHeight_);
}

/** Getters **/
const SDL_Rect& Chipset::getTile(int index) const {
	if (index < 0 || index >= static_cast<int>(tiles_.size())) throw std::out_of_range("Index tuile invalide : " + std::to_string(index));
	return tiles_[index];
}