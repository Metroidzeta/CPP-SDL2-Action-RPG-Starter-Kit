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
#include <SDL2/SDL.h>

#include "RendererContext.hpp"
#include "MapRenderer.hpp"
#include "HeroRenderer.hpp"
#include "HudRenderer.hpp"
#include "../../headers/Config.hpp"
#include "../../headers/Camera.hpp"
#include "../../headers/Hero.hpp"
#include "../../resources/headers/Font.hpp"
#include "../../resources/headers/Texture.hpp"

/**
 * Gestionnaire central de l'affichage du moteur de jeu.
 */
class Renderer final {
private:
	inline static constexpr SDL_Rect BACKGROUND_RECT{0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT};

	RendererContext ctx_;
	MapRenderer mapRenderer_;
	HeroRenderer heroRenderer_;
	HudRenderer hudRenderer_;

public:
	/** Constructeur **/
	Renderer(
		SDL_Renderer* renderer, Camera& camera, Hero& hero, const Font& fpsFont, const Font& baseFont,
		const Texture& flasksTexture, const Texture& xpBarTexture, float scaleX, float scaleY
	);

	/** Copie interdite **/
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	/** Move interdit **/
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	/** Getters **/
	RendererContext& getContext() noexcept { return ctx_; }

	/** Setters **/
	void setFonts(const Font& defaultFont, const Font& fpsFont) noexcept { ctx_.setFonts(defaultFont, fpsFont); }
	void setWindowTransform(float scaleX, float scaleY, int offsetX, int offsetY) noexcept { ctx_.setWindowTransform(scaleX, scaleY, offsetX, offsetY); }

	/** Reset **/
	void clearTextCache() const;

	/** Autres méthodes de dessin **/
	void drawBlackBackground() const;

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

	/** Affichage - Héros **/
	void drawHero() const;
	void drawHeroName() const;
	void drawHeroSwordHitbox() const;
	void drawHeroOverheadMessage(std::string_view msg) const;

	/** Affichage - Carte **/
	void updateMapDisplayBounds();
	void drawLayer(int layerIndex) const;
	void drawWalls() const;
};