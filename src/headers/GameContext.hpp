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

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>

#include "AudioManager.hpp"
#include "Camera.hpp"
#include "Hero.hpp"

class GameContext final {
private:
	Hero& hero_;
	AudioManager audioManager_;
	Camera camera_;

	std::u32string message_, lastMessage_;
	std::size_t nbEventPass_ = 0;

public:
	/** Constructeur **/
	GameContext(Hero& hero, std::unordered_map<std::string, SoundEffect*> soundEffects)
		: hero_(hero), audioManager_(std::move(soundEffects)) {}

	/** Copie interdite **/
	GameContext(const GameContext&) = delete;
	GameContext& operator=(const GameContext&) = delete;

	/** Move interdit **/
	GameContext(GameContext&&) = delete;
	GameContext& operator=(GameContext&&) = delete;

	/** Getters **/
	Hero&                 getHero()              noexcept { return hero_; }
	AudioManager&         getAudioManager()      noexcept { return audioManager_; }
	Camera&               getCamera()            noexcept { return camera_; }
	std::size_t           getNbEventPass() const noexcept { return nbEventPass_; }
	std::u32string&       getMessage()           noexcept { return message_; }
	const std::u32string& getLastMessage() const noexcept { return lastMessage_; }

	/** Méthodes utilitaires **/
	void incrementNbEventPass() noexcept { ++nbEventPass_; }
	void resetNbEventPass() noexcept { nbEventPass_ = 0; }

	void resetCamera() noexcept {
		camera_.setTarget(&hero_);
		camera_.update(); // recalculer immédiatement l'offset caméra
		camera_.sync(); // synchroniser la caméra pour empêcher un glissement
	}

	void clearMessage() noexcept { message_.clear(); }
	void saveMessage() { lastMessage_.clear(); lastMessage_.append(message_); }
	void restoreLastMessage() { message_.clear(); message_.append(lastMessage_); }
	void deleteLastCharacterMessage() { if (!message_.empty()) message_.pop_back(); }
};