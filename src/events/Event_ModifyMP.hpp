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
 * Événement : modification des MP (points de magie / mana).
 * Valeur peut être positive (gain) ou négative (perte).
 */
class Event_ModifyMP final : public Event {
private:
	const int mp_;

public:
	/** Constructeur **/
	explicit Event_ModifyMP(int mp) : mp_(mp) { if (mp == 0)  throw std::invalid_argument("Modifier les MP de 0 ne sert a rien"); }

	/** Autres méthodes **/
	void execute(GameContext& gc) const override { gc.getHero().addMp(mp_); }
};