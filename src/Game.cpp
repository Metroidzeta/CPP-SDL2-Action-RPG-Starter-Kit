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
#include <iostream>
#include <sstream> // std::ostringstream
#include <filesystem>
#include <fstream>

#include "loaders/headers/LoadUiAssets.hpp"
#include "loaders/headers/LoadSkins.hpp"
#include "loaders/headers/LoadFonts.hpp"
#include "loaders/headers/LoadMusics.hpp"
#include "loaders/headers/LoadSoundEffects.hpp"
#include "loaders/headers/LoadChipsets.hpp"
#include "loaders/headers/LoadGameMaps.hpp"
#include "loaders/headers/LoadEvents.hpp"
#include "headers/Config.hpp"
#include "headers/Util.hpp"
#include "headers/Game.hpp"

namespace {
	std::unordered_map<std::string, SoundEffect*> makeSoundEffectPointers(std::unordered_map<std::string, SoundEffect>& soundEffects) {
		std::unordered_map<std::string, SoundEffect*> result;
		result.reserve(soundEffects.size());

		for (auto& [name, sound] : soundEffects) {
			result.emplace(name, &sound);
		}

		return result;
	}

	std::string loadHeroNameFromFile(const std::filesystem::path& path) {
		try {
			std::ifstream file(path);
			if (!file) throw std::runtime_error("impossible d'ouvrir le fichier");

			std::ostringstream buffer;
			buffer << file.rdbuf();

			std::string name = Util::trim(buffer.str());
			if (name.empty()) throw std::runtime_error("fichier vide");

			return name;
		}
		catch (const std::exception& e) {
			std::cerr << "Erreur lecture du fichier " << path.string()<< " : " << e.what() << " -> Nom par défaut : Test\n";
			return "Test";
		}
	}
}

Game::Game()
	: gameWindow_(Config::WINDOW_TITLE, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), // 2 derniers booléens possibles : fullscreen / vsync
	uiAssets_(LoadUiAssets::get(gameWindow_.getRenderer())),
	skins_(LoadSkins::get(gameWindow_.getRenderer())), fonts_(LoadFonts::get(gameWindow_.getUniformScale())),
	musics_(LoadMusics::get()), soundEffects_(LoadSoundEffects::get()),
	chipsets_(LoadChipsets::get(gameWindow_.getRenderer())),
	gameMaps_(LoadGameMaps::get(*this)),
	hero_(loadHeroNameFromFile("PSEUDO.txt"), getSkin("Evil.png"), HeroClass::ROGUE, 1, 1000, 12, 12, getGameMap("Chateau_Roland_Cour_Interieure"), 10),
	gc_(hero_, makeSoundEffectPointers(soundEffects_)),
	renderer_(gameWindow_.getRenderer(), gc_.getCamera(), hero_, fonts_.at("FPS"), fonts_.at("Base"), uiAssets_.at("Fioles"), uiAssets_.at("BarreXp"), gameWindow_.getUniformScale(), gameWindow_.getUniformScale()),
	gameLoop_(gc_, gameWindow_, renderer_, controls_, fonts_) {
	LoadEvents::inject(*this);
	gc_.resetCamera();
	controls_.setTargets(&hero_, &gc_.getMessage());
	renderer_.setWindowTransform(gameWindow_.getUniformScale(), gameWindow_.getUniformScale(), gameWindow_.getOffsetX(), gameWindow_.getOffsetY());
}

void Game::play() { gameLoop_.start(); }