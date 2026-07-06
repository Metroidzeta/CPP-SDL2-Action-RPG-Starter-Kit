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

#include <cstddef>    // std::size_t
#include <functional> // std::hash
#include <ostream>    // std::ostream
#include <string>     // std::string
#include <SDL2/SDL.h> // SDL_Rect

#include "Config.hpp"

/**
 * Position 2D (x,y)
 * Utilisée pour le héros
 */
class Position {
private:
	int x_, y_;

public:
	/** Constructeur **/
	Position(int x, int y) noexcept : x_(x), y_(y) {}

	/** Getters **/
	int getX() const noexcept { return x_; }
	int getY() const noexcept { return y_; }

	/** Setters **/
	void set(int x, int y) noexcept { x_ = x; y_ = y; }
	void move(int dx, int dy) noexcept { x_ += dx; y_ += dy; }

	/** Méthodes utilitaires **/
	SDL_Rect getRectHitbox(int size) const noexcept { return SDL_Rect{ x_, y_, size, size }; }

	/** ToString() **/
	std::string toString()      const { return "(" + std::to_string(x_) + "," + std::to_string(y_) + ")"; }
	std::string toStringCells() const { return "(" + std::to_string(x_ / Config::CELL_SIZE) + "," + std::to_string(y_ / Config::CELL_SIZE) + ")"; }

	/** Égalité **/
	bool operator==(const Position& other) const noexcept { return x_ == other.x_ && y_ == other.y_; }
};

/** Opérateur d'affichage **/
inline std::ostream& operator<<(std::ostream& os, const Position& pos) {
	return os << pos.toString();
}

/** Hash pour unordered_map<Position, ...> **/
namespace std {
	template <>
	struct hash<Position> {
		std::size_t operator()(const Position& p) const noexcept {
			std::size_t seed = std::hash<int>{}(p.getX());
			seed ^= std::hash<int>{}(p.getY())
				  + static_cast<std::size_t>(0x9e3779b9u)
				  + (seed << 6)
				  + (seed >> 2);

			return seed;
		}
	};
}