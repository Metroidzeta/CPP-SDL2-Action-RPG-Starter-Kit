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
#include <algorithm>
#include <utility> // std::move
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

#include "headers/JsonUtil.hpp"
#include "headers/LoadGameMaps.hpp"
#include "../headers/Game.hpp"
#include "../headers/Chipset.hpp"
#include "../resources/headers/Music.hpp"
#include "../headers/Config.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;
using JsonUtil::isIntKey;
using JsonUtil::isStringKey;

namespace {
	std::string extractGameMapName(const fs::path& path, const std::string& suffix) {
		const std::string fileName = path.filename().string();
		if (fileName.size() >= suffix.size() && fileName.compare(fileName.size() - suffix.size(), suffix.size(), suffix) == 0) {
			return fileName.substr(0, fileName.size() - suffix.size());
		}
		return fileName;
	}
}

GameMap::IntMatrix LoadGameMaps::matrixHasValidDimensions(GameMap::IntMatrix matrix, int height, int width, const std::string& gameMapName, const char* layerName) {
	if (matrix.empty()) throw std::invalid_argument("[ERREUR] " + gameMapName + " : " + std::string(layerName) + " manquante ou invalide");
	if ((int) matrix.size() != height) {
		 throw std::invalid_argument(
			"[ERREUR] " + gameMapName + " : hauteur de " + std::string(layerName) +
			" incorrecte (" + std::to_string(matrix.size()) +
			" au lieu de " + std::to_string(height) + ")"
		);
	}

	for (int i = 0; i < height; ++i) {
		if ((int) matrix[i].size() != width) {
			throw std::invalid_argument(
				"[ERREUR] " + gameMapName + " : largeur de " + std::string(layerName) +
				" incorrecte à la ligne " + std::to_string(i) +
				" (" + std::to_string(matrix[i].size()) +
				" au lieu de " + std::to_string(width) + ")"
			);
		}
	}
	return matrix;
}

std::vector<std::vector<bool>> LoadGameMaps::convertWallsToBooleans(const GameMap::IntMatrix& src, int height, int width) {
	std::vector<std::vector<bool>> dst(static_cast<std::size_t>(height), std::vector<bool>(static_cast<std::size_t>(width), false));

	const int h = std::min(height, static_cast<int>(src.size()));
	for (int i = 0; i < h; ++i) {
		const int w = std::min(width, static_cast<int>(src[i].size()));
		for (int j = 0; j < w; ++j) {
			dst[i][j] = (src[i][j] == 1);
		}
	}
	return dst;
}

std::vector<std::vector<bool>> LoadGameMaps::readWalls(const std::string& gameMapName, int height, int width) {
	const fs::path fileME = fs::path(Config::GAME_MAP_DIRECTORY) / (gameMapName + "_ME.json");
	std::vector<std::vector<bool>> walls(static_cast<std::size_t>(height), std::vector<bool>(static_cast<std::size_t>(width), false));

	if (!fs::exists(fileME)) return walls;
	try {
		std::ifstream f(fileME, std::ios::in);
		if (!f) throw std::runtime_error("impossible d'ouvrir le fichier");

		json root;
		f >> root;

		auto wallsInt = matrixHasValidDimensions(root.at("murs").get<GameMap::IntMatrix>(), height, width, gameMapName, "murs");
		return convertWallsToBooleans(wallsInt, height, width);
	} catch (const std::exception& e) {
		std::cerr << "[ERREUR] Lecture murs de " << gameMapName << " : " << e.what() << "\n";
		return walls;
	}
}

void LoadGameMaps::readJSONFile(const fs::path& path, std::vector<LoadGameMaps::GameMapData>& gameMapsData) {
	const std::string gameMapName = extractGameMapName(path, "_BC.json");

	try {
		std::ifstream f(path, std::ios::in);
		if (!f) {
			std::cerr << "[ERREUR] Fichier " << path.filename().string() << " : impossible d'ouvrir.\n";
			return;
		}

		json root;
		f >> root;

		if (!isIntKey(root, "largeur") || !isIntKey(root, "hauteur") || !isStringKey(root, "chipset") || !isStringKey(root, "musique")) {
			throw std::invalid_argument("[ERREUR] Fichier " + gameMapName + " incomplet ou invalide (champs manquants)");
		}

		const int width = root["largeur"].get<int>();
		const int height = root["hauteur"].get<int>();
		if (width < 1 || width > GameMap::MATRIX_SIZE_MAX || height < 1 || height > GameMap::MATRIX_SIZE_MAX) {
			throw std::invalid_argument("[ERREUR] " + gameMapName + " : dimensions invalides");
		}
		const std::string chipsetName = root["chipset"].get<std::string>();
		const std::string musicName = root["musique"].get<std::string>();

		auto l0 = matrixHasValidDimensions(root.at("couche0").get<GameMap::IntMatrix>(), height, width, gameMapName, "couche0");
		auto l1 = matrixHasValidDimensions(root.at("couche1").get<GameMap::IntMatrix>(), height, width, gameMapName, "couche1");
		auto l2 = matrixHasValidDimensions(root.at("couche2").get<GameMap::IntMatrix>(), height, width, gameMapName, "couche2");

		auto walls = readWalls(gameMapName, height, width);

		LoadGameMaps::GameMapData gmd;
		gmd.gameMapName = gameMapName;
		gmd.width       = width;
		gmd.height      = height;
		gmd.chipsetName = chipsetName;
		gmd.musicName   = musicName;
		gmd.l0 = std::move(l0);
		gmd.l1 = std::move(l1);
		gmd.l2 = std::move(l2);
		gmd.walls = std::move(walls);

		gameMapsData.push_back(std::move(gmd));
	} catch (const std::exception& e) {
		std::cerr << "[ERREUR] Fichier " << path.filename().string() << " : " << e.what() << "\n";
	}
}

std::vector<LoadGameMaps::GameMapData> LoadGameMaps::getGameMapsData() {
	std::vector<LoadGameMaps::GameMapData> gameMapsData;

	const fs::path gameMapDirectory = fs::path(Config::GAME_MAP_DIRECTORY);
	if (!fs::exists(gameMapDirectory) || !fs::is_directory(gameMapDirectory)) return gameMapsData;

	try {
		for (const auto& entry : fs::directory_iterator(gameMapDirectory)) {
			if (!entry.is_regular_file()) continue;

			const auto& p = entry.path();
			const std::string fileName = p.filename().string();

			if (fileName.size() > 8 && fileName.ends_with("_BC.json")) readJSONFile(p, gameMapsData);
		}
	} catch (const std::exception& e) {
		throw std::invalid_argument(std::string("[ERREUR] Lecture des fichiers de cartes : ") + e.what());
	}

	return gameMapsData;
}

void LoadGameMaps::addGameMap(LoadGameMaps::GameMapData& elem, std::unordered_map<std::string, GameMap>& gameMaps, Game& game) {
	if (!game.hasChipset(elem.chipsetName)) {
		throw std::invalid_argument("[ERREUR] Chipset \"" + elem.chipsetName + "\" introuvable pour la carte \"" + elem.gameMapName + "\"");
	}
	if (!game.hasMusic(elem.musicName)) {
		throw std::invalid_argument("[ERREUR] Musique \"" + elem.musicName + "\" introuvable pour la carte \"" + elem.gameMapName + "\"");
	}

	Chipset& chipset = game.getChipset(elem.chipsetName);
	Music& music = game.getMusic(elem.musicName);

	gameMaps.try_emplace(elem.gameMapName, elem.gameMapName, elem.width, elem.height, chipset, &music,
		std::move(elem.l0), std::move(elem.l1), std::move(elem.l2), std::move(elem.walls));
}

/** Charge et retourne toutes les cartes **/
std::unordered_map<std::string, GameMap> LoadGameMaps::get(Game& game) {
	auto gameMapsData = getGameMapsData();

	std::unordered_map<std::string, GameMap> gameMaps;
	gameMaps.reserve(gameMapsData.size());

	for (auto& elem : gameMapsData) {
		addGameMap(elem, gameMaps, game);
	}
	return gameMaps;
}