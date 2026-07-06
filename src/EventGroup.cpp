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
#include "headers/EventGroup.hpp"

/** Ajout d'un event dans une page **/
void EventGroup::add(std::size_t page, std::unique_ptr<Event> ev) {
	if (!ev) throw std::invalid_argument("Event ajouté est NULL");

	if (page > eventsPages_.size()) { // Interdit de sauter des pages
		throw std::invalid_argument("Impossible d’ajouter un event sur une page inexistante : seule la page suivante est autorisée");
	}

	if (page == eventsPages_.size()) eventsPages_.emplace_back(); // nouvelle page vide
	eventsPages_[page].push_back(std::move(ev)); // Ajout de l'event dans la bonne page
}

/** Retourne un event si la page + index existe, sinon nullptr **/
Event* EventGroup::getEventIfExists(std::size_t page, std::size_t index) const noexcept {
	if (page >= eventsPages_.size()) return nullptr;
	const auto& events = eventsPages_[page];
	if (index >= events.size()) return nullptr;
	return events[index].get();
}

/** Si la liste d'événements est terminée **/
bool EventGroup::isFinished(std::size_t page, std::size_t index) const noexcept {
	if (page >= eventsPages_.size()) return true;
	return index >= eventsPages_[page].size();
}