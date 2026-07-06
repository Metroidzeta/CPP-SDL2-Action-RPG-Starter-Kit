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
#include <SDL2/SDL.h>

class Surface;

class Texture final {
private:
	SDL_Texture* tex_ = nullptr;

public:
	/** Constructeurs **/
	Texture(SDL_Renderer* renderer, const std::string& path);
	Texture(SDL_Renderer* renderer, const Surface& surface);

	/** Destructeur **/
	~Texture() noexcept;

	/** Copie interdite **/
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	/** Move constructor **/
	Texture(Texture&& other) noexcept;

	/** Move assignment **/
	Texture& operator=(Texture&& other) noexcept;

	/** Méthodes utilitaires **/
	void querySize(int& width, int& height) const; // Renvoie la largeur et la hauteur de la texture en pixels

	/** Getters **/
	SDL_Texture* get() const noexcept { return tex_; }
};