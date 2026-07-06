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
#include <stdexcept>
#include <utility>

#include "../headers/Config.hpp"
#include "../headers/Util.hpp"
#include "headers/Music.hpp"

/** Constructeur **/
Music::Music(const std::string& fileName)
	: name_(fileName) {
	if (name_.empty() || Util::isBlank(name_)) throw std::invalid_argument("Nom du fichier de la musique vide");

	const std::string path = std::string(Config::MUSIC_DIRECTORY) + '/' + fileName;
	track_ = Mix_LoadMUS(path.c_str());
	if (!track_) throw std::runtime_error("Echec chargement musique: " + fileName);
}

/** Destructeur **/
Music::~Music() noexcept { Mix_FreeMusic(track_); }

/** Move constructor **/
Music::Music(Music&& other) noexcept
	: name_(std::move(other.name_)), track_(std::exchange(other.track_, nullptr)), isPlaying_(other.isPlaying_), isPaused_(other.isPaused_) {
	other.isPlaying_ = false;
	other.isPaused_ = false;
}

/** Move assignment **/
Music& Music::operator=(Music&& other) noexcept {
	if (this != &other) {
		Mix_FreeMusic(track_); // Libère ancienne ressource
		name_ = std::move(other.name_); // Move du nom
		track_ = std::exchange(other.track_, nullptr); // Move de la piste

		isPlaying_ = other.isPlaying_;
		isPaused_ = other.isPaused_;

		other.isPlaying_ = false;
		other.isPaused_ = false;
	}
	return *this;
}

/** Actions **/
void Music::play() {
	if (!track_ || isPlaying_) return;
	Mix_PlayMusic(track_, -1);
	isPlaying_ = true;
	isPaused_ = false;
}

void Music::stop() {
	if (!track_) return;
	Mix_HaltMusic();
	isPlaying_ = false;
	isPaused_ = false;
}

void Music::pause() {
	if (!track_) return;
	if (isPlaying_ && !isPaused_) {
		Mix_PauseMusic();
		isPaused_ = true;
	}
}

void Music::resume() {
	if (!track_) return;
	if (isPaused_) {
		Mix_ResumeMusic();
		isPaused_ = false;
	}
}