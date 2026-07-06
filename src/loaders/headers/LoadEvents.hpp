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

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "../../events/Event.hpp"

class Game;

class LoadEvents final {
private:
	struct EventData {
		std::string gameMapName;
		int xCell = 0;
		int yCell = 0;
		std::unique_ptr<Event> event;
	};

	LoadEvents() = delete;

	static std::vector<EventData> getEvents(Game& game);
	static std::string extractGameMapName(const std::filesystem::path& path);
	static void readJsonFile(const std::filesystem::path& path, Game& game, std::vector<EventData>& events);
	static void addEvent(EventData& elem, Game& game);

public:
	static void inject(Game& game);
};