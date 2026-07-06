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
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <SDL2/SDL.h>

#include "headers/MapRenderer.hpp"
#include "../headers/Camera.hpp"
#include "../headers/Hero.hpp"
#include "../headers/GameMap.hpp"
#include "../headers/Chipset.hpp"
#include "../headers/Config.hpp"
#include "headers/RendererContext.hpp"

/** Constructeur **/
MapRenderer::MapRenderer(Camera& camera, Hero& hero, RendererContext& ctx) noexcept
	: camera_(camera), hero_(hero), ctx_(ctx) {}

/** Calcule les bornes d'affichage visibles par la caméra. **/
void MapRenderer::calculateDisplayBounds() {
	const int xCellCam = static_cast<int>(std::floor(-camera_.getX() / static_cast<double>(Config::CELL_SIZE)));
	const int yCellCam = static_cast<int>(std::floor(-camera_.getY() / static_cast<double>(Config::CELL_SIZE)));
	const GameMap& gameMap = hero_.getCurrentGameMap();

	bounds_[X0] = std::max(xCellCam - 1, 0); // x0
	bounds_[X1] = std::min(xCellCam + Config::WINDOW_WIDTH_CELLS + 2, gameMap.getWidth()); // x1
	bounds_[Y0] = std::max(yCellCam - 1, 0); // y0
	bounds_[Y1] = std::min(yCellCam + Config::WINDOW_HEIGHT_CELLS + 2, gameMap.getHeight()); // y1

	if (Config::DEBUG_MODE) std::printf("x0: %d, x1: %d, y0: %d, y1: %d\n", bounds_[X0], bounds_[X1], bounds_[Y0], bounds_[Y1]);
}

/** Affichage - Carte **/
void MapRenderer::drawLayer(int layerIndex) const {
	const int xCam = static_cast<int>(camera_.getX()), yCam = static_cast<int>(camera_.getY());
	const GameMap& gameMap = hero_.getCurrentGameMap();
	const Chipset& chipset = gameMap.getChipset();

	const int cellSize = Config::CELL_SIZE;
	SDL_Rect dstRect{0, 0, cellSize, cellSize};

	for (int i = bounds_[Y0]; i < bounds_[Y1]; ++i) {
		for (int j = bounds_[X0]; j < bounds_[X1]; ++j) {
			const int numTile = gameMap.getNumTile(layerIndex, i, j) - 1;
			if (numTile >= 0) {
				dstRect.x = j * cellSize + xCam;
				dstRect.y = i * cellSize + yCam;
				ctx_.drawTexture(chipset.getTexture(), chipset.getTile(numTile), dstRect);
			}
		}
	}
}

void MapRenderer::drawWalls() const {
	const int xCam = static_cast<int>(camera_.getX()), yCam = static_cast<int>(camera_.getY());
	const GameMap& gameMap = hero_.getCurrentGameMap();

	const int cellSize = Config::CELL_SIZE;
	SDL_Rect wallRect{0, 0, cellSize, cellSize};

	for (int i = bounds_[Y0]; i < bounds_[Y1]; ++i) {
		for (int j = bounds_[X0]; j < bounds_[X1]; ++j) {
			if (gameMap.isWall(i, j)) {
				wallRect.x = j * cellSize + xCam;
				wallRect.y = i * cellSize + yCam;
				ctx_.drawRectangle(wallRect, Config::PURPLE_TRANSPARENT);
			}
		}
	}
}