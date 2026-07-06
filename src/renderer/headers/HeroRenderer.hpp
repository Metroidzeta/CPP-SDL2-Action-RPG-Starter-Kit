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

#include <string_view>
#include <optional>

#include "../../resources/headers/Texture.hpp"

class Hero;
class RendererContext;

/**
 * Gestion du rendu du héros.
 */
class HeroRenderer final {
private:
	inline static constexpr int OVERHEAD_MESSAGE_PADDING = 3;
	inline static constexpr int OVERHEAD_MESSAGE_WIDTH_CELLS = 7;

	mutable std::optional<Texture> heroNameTexture_;
	mutable int heroNameTextWidth_ = 0;
	mutable int heroNameTextHeight_ = 0;

	Hero& hero_;
	RendererContext& ctx_;

	/** Méthodes privées **/
	void drawHeroSkin(int x, int y) const;

public:
	/** Constructeur **/
	HeroRenderer(Hero& hero, RendererContext& ctx) noexcept;

	/** Méthodes publiques (Reset) **/
	void clearTextCache() const;

	/** Affichage - Héros **/
	void drawHero() const;
	void drawHeroName() const;
	void drawHeroSwordHitbox() const;
	void drawHeroOverheadMessage(std::string_view msg) const;
};