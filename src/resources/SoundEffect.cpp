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
#include <format>

#include "../headers/Config.hpp"
#include "../headers/Util.hpp"
#include "headers/SoundEffect.hpp"

/** Constructeur **/
SoundEffect::SoundEffect(const std::string& fileName)
	: name_(fileName) {
	if (name_.empty() || Util::isBlank(name_)) throw std::invalid_argument("FileName du soundEffect vide");

	const std::string path = std::string(Config::SOUND_EFFECTS_DIRECTORY) + '/' + fileName;
	chunk_ = Mix_LoadWAV(path.c_str());
	if (!chunk_) throw std::runtime_error(std::format("Echec chargement soundEffect : {} | Mix_GetError: {}", fileName, Mix_GetError()));
}

/** Destructeur **/
SoundEffect::~SoundEffect() noexcept { Mix_FreeChunk(chunk_); }

/** Move constructor **/
SoundEffect::SoundEffect(SoundEffect&& other) noexcept : name_(std::move(other.name_)), chunk_(std::exchange(other.chunk_, nullptr)) {}

/** Move assignment **/
SoundEffect& SoundEffect::operator=(SoundEffect&& other) noexcept {
	if (this != &other) {
		Mix_FreeChunk(chunk_); // Libère ancienne ressource
		name_ = std::move(other.name_); // Move du nom
		chunk_ = std::exchange(other.chunk_, nullptr); // Move du son
	}
	return *this;
}

/** Méthodes utilitaires **/
void SoundEffect::play() const noexcept {
	if (!chunk_) return;
	Mix_PlayChannel(-1, chunk_, 0); // -1 = jouer sur le premier channel disponible
}