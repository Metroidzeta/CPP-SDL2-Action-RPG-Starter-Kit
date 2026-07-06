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

#include "../resources/headers/Music.hpp"
#include "../resources/headers/SoundEffect.hpp"

class AudioManager final {
private:
	std::unordered_map<std::string, SoundEffect*> soundEffects_;
	Music* currentMusic_ = nullptr;

public:
	/** Constructeur **/
	explicit AudioManager(std::unordered_map<std::string, SoundEffect*> soundEffects);

	/** Destructeur **/
	~AudioManager() noexcept;

	/** Copie interdite **/
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	/** Move interdit **/
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator=(AudioManager&&) = delete;

	const Music* getCurrentMusic() const noexcept { return currentMusic_; }

	void playMusic(Music* music);
	void stopMusic();
	void playSound(const std::string& name);
};