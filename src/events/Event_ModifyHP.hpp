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

#include "Event.hpp"
#include "../headers/GameContext.hpp"

/**
 * Événement : modification des HP (points de vie).
 * Valeur peut être positive (gain) ou négative (perte).
 */
class Event_ModifyHP final : public Event {
private:
	const int hp_;

public:
	/** Constructeur **/
	explicit Event_ModifyHP(int hp) : hp_(hp) { if (hp == 0)  throw std::invalid_argument("Modifier les HP de 0 ne sert à rien"); }

	/** Autres méthodes **/
	void execute(GameContext& gc) const override { gc.getHero().addHp(hp_); }
};