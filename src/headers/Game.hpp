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
#include <string>
#include <unordered_map>

#include "Chipset.hpp"
#include "Controls.hpp"
#include "GameWindow.hpp"
#include "Skin.hpp"
#include "../resources/headers/Texture.hpp"
#include "../resources/headers/Font.hpp"
#include "../resources/headers/Music.hpp"
#include "../resources/headers/SoundEffect.hpp"
#include "GameMap.hpp"
#include "Hero.hpp"
#include "GameContext.hpp"
#include "../renderer/headers/Renderer.hpp"
#include "GameLoop.hpp"

/**
 * Classe principale du moteur de jeu temps réel.
 * Charge les données, gère la boucle de rendu, les entrées, les événements et le rafraîchissement graphique.
 */
class Game final {
private:
	Controls& controls_ = Controls::getInstance();
	GameWindow gameWindow_;

	std::unordered_map<std::string, Texture> uiAssets_;
	std::unordered_map<std::string, Skin> skins_;
	std::unordered_map<std::string, Font> fonts_;
	std::unordered_map<std::string, Music> musics_;
	std::unordered_map<std::string, SoundEffect> soundEffects_;
	std::unordered_map<std::string, Chipset> chipsets_;
	std::unordered_map<std::string, GameMap> gameMaps_;

	Hero hero_;
	GameContext gc_;
	Renderer renderer_;
	GameLoop gameLoop_;

public:
	/** Constructeur **/
	Game();

	/** Copie interdite **/
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	/** Getters **/
	Skin& getSkin(const std::string& name) {
		auto it = skins_.find(name);
		if (it == skins_.end()) throw std::out_of_range("Skin introuvable : " + name);
		return it->second;
	}
	Music& getMusic(const std::string& name) {
		auto it = musics_.find(name);
		if (it == musics_.end()) throw std::out_of_range("Musique introuvable : " + name);
		return it->second;
	}
	SoundEffect& getSoundEffect(const std::string& name) {
		auto it = soundEffects_.find(name);
		if (it == soundEffects_.end()) throw std::out_of_range("Bruitage introuvable : " + name);
		return it->second;
	}
	Chipset& getChipset(const std::string& name) {
		auto it = chipsets_.find(name);
		if (it == chipsets_.end()) throw std::out_of_range("Chipset introuvable : " + name);
		return it->second;
	}
	GameMap& getGameMap(const std::string& name) {
		auto it = gameMaps_.find(name);
		if (it == gameMaps_.end()) throw std::out_of_range("Carte introuvable : " + name);
		return it->second;
	}

	bool hasGameMap(const std::string& name) const { return gameMaps_.contains(name); }
	bool hasMusic(const std::string& name)   const { return musics_.contains(name); }
	bool hasChipset(const std::string& name) const { return chipsets_.contains(name); }

	/** Méthodes utilitaires **/
	void play();
};