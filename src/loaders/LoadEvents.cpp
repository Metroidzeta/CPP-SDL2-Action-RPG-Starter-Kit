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
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

#include "headers/JsonUtil.hpp"
#include "headers/LoadEvents.hpp"
#include "../headers/Config.hpp"
#include "../headers/Game.hpp"
#include "../headers/GameMap.hpp"
#include "../resources/headers/Music.hpp"
#include "../events/Event_AM.hpp"
#include "../events/Event_JM.hpp"
#include "../events/Event_LVLUP.hpp"
#include "../events/Event_MSG.hpp"
#include "../events/Event_ModifyHP.hpp"
#include "../events/Event_ModifyMP.hpp"
#include "../events/Event_TP.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;
using JsonUtil::isArrayKey;
using JsonUtil::isIntKey;
using JsonUtil::isStringKey;

std::string LoadEvents::extractGameMapName(const fs::path& path) {
	const std::string fileName = path.filename().string();
	constexpr std::string_view suffix = "_ME.json";

	if (fileName.size() >= suffix.size()
		&& fileName.compare(fileName.size() - suffix.size(), suffix.size(), suffix) == 0) {
		return fileName.substr(0, fileName.size() - suffix.size());
	}

	return fileName;
}

std::vector<LoadEvents::EventData> LoadEvents::getEvents(Game& game) {
	std::vector<EventData> events;

	try {
		const fs::path gameMapDirectory = fs::path(Config::GAME_MAP_DIRECTORY);
		if (!fs::exists(gameMapDirectory) || !fs::is_directory(gameMapDirectory)) return events;

		for (const auto& entry : fs::directory_iterator(gameMapDirectory)) {
			if (!entry.is_regular_file()) continue;

			const fs::path& path = entry.path();
			const std::string fileName = path.filename().string();

			if (fileName.size() > 8 && fileName.ends_with("_ME.json")) {
				readJsonFile(path, game, events);
			}
		}
	} catch (const std::exception& e) {
		throw std::invalid_argument(std::string("[ERREUR] Lecture des fichiers d'evenements : ") + e.what());
	}

	return events;
}

void LoadEvents::readJsonFile(const fs::path& path, Game& game, std::vector<EventData>& events) {
	try {
		std::ifstream file(path, std::ios::in);
		if (!file) throw std::runtime_error("impossible d'ouvrir le fichier");

		json root;
		file >> root;

		if (!isArrayKey(root, "ensemblesEvents")) return;

		const json& eventGroups = root.at("ensemblesEvents");
		if (eventGroups.empty()) return;

		const std::string gameMapName = extractGameMapName(path);

		for (const json& block : eventGroups) {
			if (!block.is_object() || !isIntKey(block, "x") || !isIntKey(block, "y") || !isArrayKey(block, "events")) {
				std::cerr << "[ERREUR] Bloc d'evenement incomplet ignore dans " << path.filename().string() << '\n';
				continue;
			}

			const int xCell = block.at("x").get<int>();
			const int yCell = block.at("y").get<int>();
			const json& eventArray = block.at("events");

			if (eventArray.empty()) continue;

			for (const json& ev : eventArray) {
				if (!ev.is_object() || !isStringKey(ev, "type")) {
					std::cerr << "[ERREUR] Evenement sans type ignore (" << path.filename().string() << " [" << xCell << "," << yCell << "])\n";
					continue;
				}

				const std::string type = ev.at("type").get<std::string>();

				if (type == "MSG") {
					if (!isStringKey(ev, "texte")) {
						std::cerr << "[AVERTISSEMENT] MSG sans texte ignore dans " << path.filename().string() << '\n';
						continue;
					}

					events.push_back(EventData{gameMapName, xCell, yCell, std::make_unique<Event_MSG>(ev.at("texte").get<std::string>())});
				}
				else if (type == "TP") {
					if (!isIntKey(ev, "xDst") || !isIntKey(ev, "yDst") || !isStringKey(ev, "carteDst")) {
						std::cerr << "[AVERTISSEMENT] TP incomplet ignore dans " << path.filename().string() << '\n';
						continue;
					}

					const int xCellDst = ev.at("xDst").get<int>();
					const int yCellDst = ev.at("yDst").get<int>();
					const std::string gameMapDstName = ev.at("carteDst").get<std::string>();

					if (!game.hasGameMap(gameMapDstName)) {
						std::cerr << "[AVERTISSEMENT] Carte de destination introuvable \"" << gameMapDstName << "\" dans " << path.filename().string() << '\n';
						continue;
					}

					events.push_back(EventData{gameMapName, xCell, yCell, std::make_unique<Event_TP>(xCellDst, yCellDst, game.getGameMap(gameMapDstName))});
				}
				else if (type == "JouerMusique") {
					if (!isStringKey(ev, "nom")) {
						std::cerr << "[AVERTISSEMENT] JouerMusique sans nom de musique ignore dans " << path.filename().string() << '\n';
						continue;
					}

					const std::string musicName = ev.at("nom").get<std::string>();
					if (!game.hasMusic(musicName)) {
						std::cerr << "[AVERTISSEMENT] Musique introuvable \"" << musicName << "\" dans " << path.filename().string() << '\n';
						continue;
					}

					events.push_back(EventData{gameMapName, xCell, yCell, std::make_unique<Event_JM>(game.getMusic(musicName))});
				}
				else if (type == "ArretMusique") {
					events.push_back(EventData{gameMapName, xCell, yCell, std::make_unique<Event_AM>()});
				}
				else if (type == "PV") {
					if (!isIntKey(ev, "valeur")) {
						std::cerr << "[AVERTISSEMENT] modifyHP sans valeur ignore dans " << path.filename().string() << '\n';
						continue;
					}

					events.push_back(EventData{gameMapName, xCell, yCell, std::make_unique<Event_ModifyHP>(ev.at("valeur").get<int>())});
				}
				else if (type == "PM") {
					if (!isIntKey(ev, "valeur")) {
						std::cerr << "[AVERTISSEMENT] modifyMP sans valeur ignore dans " << path.filename().string() << '\n';
						continue;
					}

					events.push_back(EventData{gameMapName, xCell, yCell, std::make_unique<Event_ModifyMP>(ev.at("valeur").get<int>())});
				}
				else if (type == "LVLUP") {
					events.push_back(EventData{gameMapName, xCell, yCell, std::make_unique<Event_LVLUP>()});
				}
				else {
					std::cerr << "[AVERTISSEMENT] Type inconnu \"" << type << "\" ignore dans " << path.filename().string() << '\n';
				}
			}
		}
	} catch (const std::exception& e) {
		std::cerr << "[ERREUR] Fichier " << path.string() << " : " << e.what() << '\n';
	}
}

void LoadEvents::addEvent(EventData& elem, Game& game) {
	if (!elem.event) throw std::invalid_argument("[ERREUR] Event null");

	if (!game.hasGameMap(elem.gameMapName)) {
		throw std::invalid_argument("[ERREUR] Carte introuvable : \"" + elem.gameMapName + "\" pour y inserer un event");
	}

	game.getGameMap(elem.gameMapName).addEvent(0, elem.xCell, elem.yCell, std::move(elem.event));
}

void LoadEvents::inject(Game& game) {
	auto events = getEvents(game);

	for (auto& elem : events) {
		addEvent(elem, game);
	}
}