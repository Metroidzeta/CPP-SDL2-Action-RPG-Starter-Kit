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
#include <stdexcept>
#include <utility>

#include "headers/AudioManager.hpp"
#include "headers/Util.hpp"

AudioManager::AudioManager(std::unordered_map<std::string, SoundEffect*> soundEffects)
	: soundEffects_(std::move(soundEffects)) {
	for (const auto& [name, sound] : soundEffects_) {
		if (name.empty() || Util::isBlank(name)) throw std::invalid_argument("SoundEffect name est vide");
		if (!sound) throw std::invalid_argument("Sound effect est null " + name);
	}
}

/** Destructeur **/
AudioManager::~AudioManager() noexcept { stopMusic(); }

void AudioManager::playMusic(Music* music) {
	if (currentMusic_ == music) return;
	stopMusic();
	currentMusic_ = music;
	if (currentMusic_) currentMusic_->play();
}

void AudioManager::stopMusic() {
	if (!currentMusic_) return;
	currentMusic_->stop();
	currentMusic_ = nullptr;
}

void AudioManager::playSound(const std::string& name) {
	const auto it = soundEffects_.find(name);
	SoundEffect* sound = (it != soundEffects_.end()) ? it->second : nullptr;

	if (!sound) {
		std::cerr << "Missing sound effect: " << name << '\n';
		return;
	}

	sound->play();
}