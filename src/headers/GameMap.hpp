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
#include <array>
#include <vector>
#include <unordered_map>
#include <cstddef> // std::size_t
#include <memory> // std::unique_ptr
#include <functional> // std::hash
#include <SDL2/SDL.h>

#include "../resources/headers/Music.hpp"
#include "Chipset.hpp"
#include "EventGroup.hpp"

/**
 * Représente une carte (niveau du jeu) composée de plusieurs couches de tuiles,
 * avec une largeur et une hauteur fixes, un chipset, une musique et des événements associés.
 */
class GameMap final {
public:
	static constexpr int MATRIX_SIZE_MAX = 100; // par défaut : 100
	static constexpr int LAYERS_COUNT = 3; // par défaut : 3

	using IntMatrix  = std::vector<std::vector<int>>;
	using BoolMatrix = std::vector<std::vector<bool>>;

private:
	struct CellCoord {
		int xCell, yCell;
		bool operator==(const CellCoord&) const noexcept = default;

		struct Hash {
			std::size_t operator()(const CellCoord& coord) const noexcept {
				return std::hash<int>{}(coord.xCell)
					^ (std::hash<int>{}(coord.yCell) << 1);
			}
		};
	};

	struct CellBounds {int x0; int x1; int y0; int y1;};
	using RectMatrix = std::array<std::array<SDL_Rect, MATRIX_SIZE_MAX>, MATRIX_SIZE_MAX>;
	static const RectMatrix GLOBAL_RECT_MATRIX; // Matrice de rectangles globale représentant les cases (partagée entre toutes les cartes)

	/** Méthodes statiques privées **/
	static RectMatrix createGlobalRectMatrix();
	static void checkArguments(const std::string& name, int width, int height);

	std::string name_;
	int width_, height_; // largeur * hauteur en cases
	const Chipset* chipset_ = nullptr; // non-owning
	Music* music_ = nullptr; // non-owning, peut être nullptr (= pas de musique)
	std::array<IntMatrix, LAYERS_COUNT> layers_; // layers_[c][i][j] : 3 couches (matrices) de numTileChipset (C0 < C1 < Héros < C2)
	BoolMatrix walls_; // walls_[i][j] : Matrice booléenne représentant les murs sur chaque case (false = pas de mur, true = mur)
	std::unordered_map<CellCoord, EventGroup, CellCoord::Hash> events_; // Events par cases

	/** Méthodes privées **/
	bool isInvalidMatrixIndex(int i, int j) const noexcept;
	CellBounds getCellBounds(const SDL_Rect& rect) const noexcept;

public:
	/** Constructeurs **/
	GameMap(const std::string& name, int width, int height, const Chipset& chipset, Music* music, IntMatrix l0, IntMatrix l1, IntMatrix l2, BoolMatrix walls);
	static GameMap EmptyGameMap(const std::string& name, int width, int height, const Chipset& chipset, Music* music);

	/** Copie interdite **/
	GameMap(const GameMap&) = delete;
	GameMap& operator=(const GameMap&) = delete;

	/** Move constructor **/
	GameMap(GameMap&&) noexcept = default;

	/** Move assignment **/
	GameMap& operator=(GameMap&&) noexcept = default;

	/** Getters **/
	const std::string& getName()    const noexcept { return name_; }
	int                getWidth()   const noexcept { return width_; }
	int                getHeight()  const noexcept { return height_; }
	const Chipset&     getChipset() const noexcept { return *chipset_; }
	Music*             getMusic()   const noexcept { return music_; }
	int                getNumTile(int layerIndex, int i, int j) const;
	bool               isWall(int i, int j) const;

	/** Méthodes utilitaires **/
	bool hasWallCollision(const SDL_Rect& rect) const noexcept;
	EventGroup* findCollidingEvents(const SDL_Rect& rect) noexcept;
	void addEvent(std::size_t numPage, int xCell, int yCell, std::unique_ptr<Event> ev);

	/** Égalité (équivalent de equals en Java) **/
	bool operator==(const GameMap& other) const;
	bool operator!=(const GameMap& other) const { return !(*this == other); }
};