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

class SoundEffect final {
private:
	std::string name_;
	Mix_Chunk* chunk_ = nullptr;

public:
	/** Constructeur **/
	explicit SoundEffect(const std::string& fileName);

	/** Destructeur **/
	~SoundEffect() noexcept;

	/** Copie interdite **/
	SoundEffect(const SoundEffect&) = delete;
	SoundEffect& operator=(const SoundEffect&) = delete;

	/** Move constructor **/
	SoundEffect(SoundEffect&& other) noexcept;

	/** Move assignment **/
	SoundEffect& operator=(SoundEffect&& other) noexcept;

	/** Méthodes utilitaires **/
	void play() const noexcept;

	/** Getters **/
	const std::string& getName()  const noexcept { return name_; }
	Mix_Chunk*         getChunk() const noexcept { return chunk_; }
};