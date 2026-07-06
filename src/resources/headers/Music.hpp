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
#include <SDL2/SDL_mixer.h>

class Music final {
private:
	std::string name_;
	Mix_Music* track_ = nullptr;
	bool isPlaying_ = false;
	bool isPaused_ = false;

public:
	/** Constructeur **/
	explicit Music(const std::string& fileName);

	/** Destructeur **/
	~Music() noexcept;

	/** Copie interdite **/
	Music(const Music&) = delete;
	Music& operator=(const Music&) = delete;

	/** Move constructor **/
	Music(Music&& other) noexcept;

	/** Move assignment **/
	Music& operator=(Music&& other) noexcept;

	/** Méthodes utilitaires **/
	void play();
	void stop();
	void pause();
	void resume();

	/** Getters **/
	const std::string& getName()   const noexcept { return name_; }
	Mix_Music*         getTrack()  const noexcept { return track_; }
	bool               isPlaying() const noexcept { return isPlaying_; }
	bool               isPaused()  const noexcept { return isPaused_; }
};