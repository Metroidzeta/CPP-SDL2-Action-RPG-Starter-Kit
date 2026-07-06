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
#include <unordered_map>
#include <filesystem>
#include <vector>

#include "../../headers/GameMap.hpp"

class Game;

class LoadGameMaps final {
public:
	struct GameMapData {
		std::string gameMapName;
		int width = 0;
		int height = 0;
		std::string chipsetName;
		std::string musicName;

		GameMap::IntMatrix l0;
		GameMap::IntMatrix l1;
		GameMap::IntMatrix l2;
		GameMap::BoolMatrix walls;
	};

private:
	LoadGameMaps() = delete;

	static GameMap::IntMatrix matrixHasValidDimensions(GameMap::IntMatrix matrix, int height, int width, const std::string& gameMapName, const char* layerName);
	static std::vector<std::vector<bool>> convertWallsToBooleans(const GameMap::IntMatrix& src, int height, int width);
	static std::vector<std::vector<bool>> readWalls(const std::string& gameMapName, int height, int width);
	static void readJSONFile(const std::filesystem::path& path, std::vector<GameMapData>& gameMapsData);
	static std::vector<GameMapData> getGameMapsData();
	static void addGameMap(GameMapData& elem, std::unordered_map<std::string, GameMap>& gameMaps, Game& game);

public:
	/** Charge et retourne toutes les cartes **/
	static std::unordered_map<std::string, GameMap> get(Game& game);
};