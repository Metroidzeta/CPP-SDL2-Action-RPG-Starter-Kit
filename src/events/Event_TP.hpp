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

#include <stdexcept>
#include <string>

#include "Event.hpp"
#include "../headers/GameMap.hpp"
#include "../headers/Config.hpp"
#include "../headers/GameContext.hpp"

/**
* Événement de téléportation.
* Stocke la carte de destination + les coordonnées x et y en pixels.
*/
class Event_TP final : public Event {
private:
	int xDst_, yDst_; // en pixels
	GameMap& gameMapDst_;

public:
	/** Constructeur **/
	Event_TP(int xCellDst, int yCellDst, GameMap& gameMapDst): gameMapDst_(gameMapDst) {
		if (xCellDst < 0 || xCellDst >= gameMapDst.getWidth() || yCellDst < 0 || yCellDst >= gameMapDst.getHeight()) {
			throw std::invalid_argument("Event_TP: Coordonnées hors limite dans la carteDst " 
			+ gameMapDst.getName() + " : " + std::to_string(xCellDst) + ", " + std::to_string(yCellDst));
		}
		xDst_ = xCellDst * Config::CELL_SIZE; // vraie valeur de x : il faut multiplier par CELL_SIZE
		yDst_ = yCellDst * Config::CELL_SIZE; // vraie valeur de y : il faut multiplier par CELL_SIZE
	}

	/** Autres méthodes **/
	void execute(GameContext& gc) const override {
		//damageVisible = 0;
		gc.getAudioManager().playMusic(gameMapDst_.getMusic());
		Hero& hero = gc.getHero();
		hero.setCurrentGameMap(gameMapDst_);
		hero.updatePosition(xDst_, yDst_);
	}

	bool operator==(const Event_TP& other) const noexcept {
		return xDst_ == other.xDst_
			&& yDst_ == other.yDst_
			&& gameMapDst_.getName() == other.gameMapDst_.getName();
	}

	bool operator!=(const Event_TP& other) const noexcept {
		return !(*this == other);
	}
};