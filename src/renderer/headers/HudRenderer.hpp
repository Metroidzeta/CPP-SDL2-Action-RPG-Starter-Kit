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

#include <array>
#include <string_view>
#include <optional>

#include "../../resources/headers/Texture.hpp"

class Hero;
class RendererContext;

/**
 * Gestion du rendu de l'interface utilisateur.
 */
class HudRenderer final {
private:
	inline static constexpr std::array<int, 10> VA{68, 73, 53, 29, 241, 203, 166, 136, 139, 62};

	mutable int cachedAlignment_ = -1;
	mutable std::optional<Texture> alignmentTexture_;

	mutable int cachedFpsResultCent_ = -1;
	mutable std::optional<Texture> fpsTexture_;

	Hero& hero_;
	RendererContext& ctx_;
	const Texture& flasksTexture_;
	const Texture& xpBarTexture_;

public:
	/** Constructeur **/
	HudRenderer(Hero& hero, RendererContext& ctx, const Texture& flasksTexture, const Texture& xpBarTexture) noexcept;

	/** Copie interdite **/
	HudRenderer(const HudRenderer&) = delete;
	HudRenderer& operator=(const HudRenderer&) = delete;

	/** Move interdit **/
	HudRenderer(HudRenderer&&) = delete;
	HudRenderer& operator=(HudRenderer&&) = delete;

	/** Reset **/
	void clearTextCache() const;

	/** Affichage - Interface **/
	void drawComputePalette() const;
	void drawFPS(double fpsResult) const;
	void drawAlignment() const;
	void drawHpFlask(int flaskFrame) const;
	void drawMpFlask(int flaskFrame) const;
	void drawXpBar() const;
	void drawTextInputBox(std::string_view message) const;

	/** Affichages - Interface menu **/
	void drawNavigationMenu() const;
	void drawStatsMenu() const;

	/** Affichages - Messages **/
	void drawEventMessage(std::string_view msg) const;
};