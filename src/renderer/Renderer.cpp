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

#include "headers/Renderer.hpp"

namespace {
	SDL_Renderer* requireRenderer(SDL_Renderer* renderer) {
		if (!renderer) throw std::invalid_argument("Renderer : renderer NULL");
		return renderer;
	}
}

/** Constructeur **/
Renderer::Renderer(SDL_Renderer* renderer, Camera& camera, Hero& hero, const Font& fpsFont, const Font& baseFont,
	const Texture& flasksTexture, const Texture& xpBarTexture, float scaleX, float scaleY)
	: ctx_(requireRenderer(renderer), baseFont, fpsFont, scaleX, scaleY), mapRenderer_(camera, hero, ctx_),
	heroRenderer_(hero, ctx_), hudRenderer_(hero, ctx_, flasksTexture, xpBarTexture) {}

/** Reset **/
void Renderer::clearTextCache() const {
	hudRenderer_.clearTextCache();
	heroRenderer_.clearTextCache();
}

/** Autres méthodes de dessin **/
void Renderer::drawBlackBackground() const { ctx_.drawRectangle(BACKGROUND_RECT, Config::BLACK); }

/** Affichage - Interface **/
void Renderer::drawComputePalette()                       const { hudRenderer_.drawComputePalette(); }
void Renderer::drawFPS(double fpsResult)                  const { hudRenderer_.drawFPS(fpsResult); }
void Renderer::drawAlignment()                            const { hudRenderer_.drawAlignment(); }
void Renderer::drawHpFlask(int flaskFrame)                const { hudRenderer_.drawHpFlask(flaskFrame); }
void Renderer::drawMpFlask(int flaskFrame)                const { hudRenderer_.drawMpFlask(flaskFrame); }
void Renderer::drawXpBar()                                const { hudRenderer_.drawXpBar(); }
void Renderer::drawTextInputBox(std::string_view message) const { hudRenderer_.drawTextInputBox(message); }

/** Affichages - Interface menu **/
void Renderer::drawNavigationMenu() const { hudRenderer_.drawNavigationMenu(); }
void Renderer::drawStatsMenu()      const { hudRenderer_.drawStatsMenu(); }

/** Affichages - Messages **/
void Renderer::drawEventMessage(std::string_view msg) const { hudRenderer_.drawEventMessage(msg); }

/** Affichage - Héros **/
void Renderer::drawHero()                                    const { heroRenderer_.drawHero(); }
void Renderer::drawHeroName()                                const { heroRenderer_.drawHeroName(); }
void Renderer::drawHeroSwordHitbox()                         const { heroRenderer_.drawHeroSwordHitbox(); }
void Renderer::drawHeroOverheadMessage(std::string_view msg) const { heroRenderer_.drawHeroOverheadMessage(msg); }

/** Affichage - Carte **/
void Renderer::updateMapDisplayBounds()        { mapRenderer_.calculateDisplayBounds(); }
void Renderer::drawLayer(int layerIndex) const { mapRenderer_.drawLayer(layerIndex); }
void Renderer::drawWalls()               const { mapRenderer_.drawWalls(); }