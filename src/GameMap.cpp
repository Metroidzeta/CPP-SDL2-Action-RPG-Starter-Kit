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
#include <algorithm> // std::max, std::min
#include <cstdio> // std::printf
#include <utility> // std::move

#include "headers/GameMap.hpp"
#include "headers/Config.hpp"
#include "headers/Util.hpp"

namespace {
	template<typename Matrix>
	bool hasDimensions(const Matrix& matrix, int width, int height) {
		if (static_cast<int>(matrix.size()) != height) return false;

		return std::all_of(matrix.begin(), matrix.end(),
			[width](const auto& row) {
				return static_cast<int>(row.size()) == width;
			}
		);
	}
}

const GameMap::RectMatrix GameMap::GLOBAL_RECT_MATRIX = GameMap::createGlobalRectMatrix();

/** Méthodes statiques privées **/
GameMap::RectMatrix GameMap::createGlobalRectMatrix() {
	RectMatrix matrix{};
	const int cellSize = Config::CELL_SIZE;
	for (int i = 0; i < MATRIX_SIZE_MAX; ++i) {
		const int y = i * cellSize;
		for (int j = 0; j < MATRIX_SIZE_MAX; ++j) {
			matrix[i][j] = { j * cellSize, y, cellSize, cellSize };
		}
	}
	return matrix;
}

void GameMap::checkArguments(const std::string& name, int width, int height) {
	if (name.empty() || Util::isBlank(name)) throw std::invalid_argument("Carte: nom null ou vide");
	if (width < 1 || width > MATRIX_SIZE_MAX) throw std::invalid_argument("Carte[" + name + "]: largeur < 1 ou > " + std::to_string(MATRIX_SIZE_MAX));
	if (height < 1 || height > MATRIX_SIZE_MAX) throw std::invalid_argument("Carte[" + name + "]: hauteur < 1 ou > " + std::to_string(MATRIX_SIZE_MAX));
}

/** Constructeurs **/
GameMap::GameMap(const std::string& name, int width, int height, const Chipset& chipset, Music* music, IntMatrix l0, IntMatrix l1, IntMatrix l2, BoolMatrix walls)
	: name_(name), width_(width), height_(height), chipset_(&chipset), music_(music) {
	checkArguments(name, width, height);

	std::array<IntMatrix, LAYERS_COUNT> srcLayers { std::move(l0), std::move(l1), std::move(l2) };
	for (int l = 0; l < LAYERS_COUNT; ++l) {
		if (srcLayers[l].empty()) {
			layers_[l] = IntMatrix(height_, std::vector<int>(width_, Chipset::EMPTY_TILE));
		} else {
			if (!hasDimensions(srcLayers[l], width_, height_)) {
				throw std::invalid_argument("Carte[" + name_ + "]: dimensions invalides pour la couche " + std::to_string(l));
			}
			layers_[l] = std::move(srcLayers[l]);
		}
	}

	if (walls.empty()) {
		walls_ = BoolMatrix(height_, std::vector<bool>(width_, false));
	} else {
		if (!hasDimensions(walls, width_, height_)) {
			throw std::invalid_argument("Carte[" + name_ + "]: dimensions invalides pour la matrice de murs");
		}
		walls_ = std::move(walls);
	}
}

GameMap GameMap::EmptyGameMap(const std::string& name, int width, int height, const Chipset& chipset, Music* music) {
	return GameMap(name, width, height, chipset, music, {}, {}, {}, {});
}

bool GameMap::isInvalidMatrixIndex(int i, int j) const noexcept { return i < 0 || i >= height_ || j < 0 || j >= width_; }

GameMap::CellBounds GameMap::getCellBounds(const SDL_Rect& rect) const noexcept {
	const int cellSize = Config::CELL_SIZE;

	return CellBounds{
		std::max(0, rect.x / cellSize),
		std::min(width_, (rect.x + rect.w - 1) / cellSize + 1),
		std::max(0, rect.y / cellSize),
		std::min(height_, (rect.y + rect.h - 1) / cellSize + 1)
	};
}

/** Getters **/
int GameMap::getNumTile(int layerIndex, int i, int j) const {
	if (layerIndex < 0 || layerIndex >= LAYERS_COUNT) throw std::out_of_range("couche < 0 ou >= " + std::to_string(LAYERS_COUNT));
	if (isInvalidMatrixIndex(i, j)) throw std::out_of_range("i < 0 ou i >= " + std::to_string(height_) + " ou j < 0 ou j >= " + std::to_string(width_));
	return layers_[layerIndex][i][j];
}

bool GameMap::isWall(int i, int j) const {
	if (isInvalidMatrixIndex(i, j)) throw std::out_of_range("i < 0 ou i >= " + std::to_string(height_) + " ou j < 0 ou j >= " + std::to_string(width_));
	return walls_[i][j];
}

/** Méthodes utilitaires **/
// --- Collisions murs ---
bool GameMap::hasWallCollision(const SDL_Rect& rect) const noexcept {
	const CellBounds b = getCellBounds(rect);
	if (Config::DEBUG_MODE) std::printf("x0: %d, x1: %d, y0: %d, y1: %d\n", b.x0, b.x1, b.y0, b.y1);

	for (int y = b.y0; y < b.y1; ++y) {
		for (int x = b.x0; x < b.x1; ++x) {
			if (walls_[y][x] && SDL_HasIntersection(&rect, &GLOBAL_RECT_MATRIX[y][x])) return true;
		}
	}
	return false;
}

// --- Collisions events ---
EventGroup* GameMap::findCollidingEvents(const SDL_Rect& rect) noexcept {
	const CellBounds b = getCellBounds(rect);

	for (int y = b.y0; y < b.y1; ++y) {
		for (int x = b.x0; x < b.x1; ++x) {
			auto it = events_.find(CellCoord{x, y});
			if (it != events_.end() && SDL_HasIntersection(&rect, &GLOBAL_RECT_MATRIX[y][x])) return &it->second;
		}
	}

	return nullptr;
}

// --- Ajouter events ---
void GameMap::addEvent(std::size_t numPage, int xCell, int yCell, std::unique_ptr<Event> ev) {
	if (isInvalidMatrixIndex(yCell, xCell)) {
		throw std::invalid_argument("Carte[" + name_ + "]: Coordonnées d'event hors limite [" + std::to_string(xCell) + "," + std::to_string(yCell) + "]");
	}
	if (!ev) throw std::invalid_argument("Event null ajouté à la carte " + name_);
	CellCoord coord{ xCell, yCell };
	auto& ensEv = events_[coord];
	ensEv.add(numPage, std::move(ev));
}

/** Égalité (équivalent de equals / hashCode en Java) **/
bool GameMap::operator==(const GameMap& other) const {
	return name_     == other.name_
		&& width_    == other.width_
		&& height_   == other.height_
		&& chipset_  == other.chipset_ // même pointeur de chipset
		&& music_    == other.music_  // même pointeur de musique (ou nullptr)
		&& layers_   == other.layers_
		&& walls_    == other.walls_;
		//&& events_ == other.events_;  // nécessite operator== sur CellCoord et EventGroup
}