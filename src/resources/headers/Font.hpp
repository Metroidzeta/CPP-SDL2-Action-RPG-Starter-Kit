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
#include <SDL2/SDL_ttf.h>

/**
 * Police TTF chargée en RAII.
 * Encapsule un TTF_Font* et le libère automatiquement avec TTF_CloseFont().
 */
class Font final {
private:
	std::string name_;
	int size_ = 0;
	TTF_Font* font_ = nullptr;

public:
	/** Constructeur **/
	Font(const std::string& fileName, int size);

	/** Destructeur **/
	~Font() noexcept;

	/** Copie interdite **/
	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	/** Move autorisé **/
	Font(Font&& other) noexcept;
	Font& operator=(Font&& other) noexcept;

	/** Getters **/
	const std::string& getName() const noexcept { return name_; }
	int getSize() const noexcept { return size_; }
	TTF_Font* get() const noexcept { return font_; }
};