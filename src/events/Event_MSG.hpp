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
#include <stdexcept>
#include <utility> // std::move

#include "Event.hpp"
#include "../headers/Util.hpp"
#include "../headers/GameContext.hpp"

class Event_MSG final : public Event {
private:
	std::string msg_;

public:
	/** Constructeur **/
	explicit Event_MSG(std::string msg): msg_(std::move(msg)) { if (msg_.empty() || Util::isBlank(msg_)) throw std::invalid_argument("Message de l'Event_MSG vide"); }

	/** Getters **/
	const std::string& getMessage() const noexcept { return msg_; }

	/** Autres méthodes **/
	void execute(GameContext& gc) const override {
		gc.getHero().setIsWriting(false);
		//clearMessage();
	}
};