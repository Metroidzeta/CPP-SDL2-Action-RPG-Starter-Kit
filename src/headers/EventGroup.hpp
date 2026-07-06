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

#include <vector>
#include <memory>
#include <cstddef>
#include <stdexcept>

#include "../events/Event.hpp"

/**
 * Représente un groupe d'événements organisés en plusieurs pages.
 * 
 * Chaque page contient une liste ordonnée d'events, permettant de structurer
 * des séquences d'actions successives dans le jeu (dialogues, téléportations, etc.).
 */
class EventGroup final {
private:
	// eventsPages_[page][index] = unique_ptr<Event> (eventsPages -> une liste de pages -> chaque page contient une liste d'événements)
	std::vector<std::vector<std::unique_ptr<Event>>> eventsPages_;

public:
	EventGroup() = default;

	/** Méthodes utilitaires **/
	void add(std::size_t page, std::unique_ptr<Event> ev); // Ajoute un event à une page donnée
	Event* getEventIfExists(std::size_t page, std::size_t index) const noexcept; // Retourne l'event si la page + index existe, sinon nullptr
	bool isFinished(std::size_t page, std::size_t index) const noexcept; // Si la liste d'événements est terminée
};