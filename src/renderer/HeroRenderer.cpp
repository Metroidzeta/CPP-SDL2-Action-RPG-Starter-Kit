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
#include <algorithm>
#include <string>

#include <SDL2/SDL.h>

#include "headers/HeroRenderer.hpp"

#include "../headers/Config.hpp"
#include "../headers/Hero.hpp"
#include "../headers/Skin.hpp"
#include "../resources/headers/Font.hpp"
#include "headers/RendererContext.hpp"

/** Constructeur **/
HeroRenderer::HeroRenderer(Hero& hero, RendererContext& ctx) noexcept
	: hero_(hero), ctx_(ctx) {}

/** Méthodes privées **/
void HeroRenderer::drawHeroSkin(int x, int y) const {
	const int regionIndex = static_cast<int>(hero_.getDirection()) * 3 + (hero_.getMoveFrame() / 4);
	hero_.getSkin().draw(ctx_.getRenderer(), regionIndex, x, y, Config::CELL_SIZE);
}

/** Méthodes publiques (Reset) **/
void HeroRenderer::clearTextCache() const {
	heroNameTexture_.reset();
	heroNameTextWidth_ = 0;
	heroNameTextHeight_ = 0;
}

/** Affichage - Héros **/
void HeroRenderer::drawHero() const {
	const int x = hero_.getXScreen(), y = hero_.getYScreen();
	drawHeroSkin(x, y); // dessiner skin du héros
}

void HeroRenderer::drawHeroName() const {
	const std::string& name = hero_.getName();
	if (name.empty()) return;
	const int x = hero_.getXScreen(), y = hero_.getYScreen();

	if (!heroNameTexture_) {
		heroNameTexture_ = ctx_.createTextureFromText(ctx_.getDefaultFont(), name, Config::WHITE);
		heroNameTexture_->querySize(heroNameTextWidth_, heroNameTextHeight_);
	}

	const float xlogicalText = x + (Config::CELL_SIZE / 2.0f);
	const float ylogicalText = y + Config::CELL_SIZE - 2.0f;

	const SDL_Rect dstRect{ctx_.toWindowX(xlogicalText) - (heroNameTextWidth_ / 2), ctx_.toWindowY(ylogicalText), heroNameTextWidth_, heroNameTextHeight_};
	ctx_.drawTexture(*heroNameTexture_, dstRect);
}

void HeroRenderer::drawHeroSwordHitbox() const { ctx_.drawRectangle(hero_.getSwordHitboxScreen(), Config::WHITE); }

void HeroRenderer::drawHeroOverheadMessage(std::string_view msg) const {
	if (msg.empty()) return;

	const int boxMaxWidth = OVERHEAD_MESSAGE_WIDTH_CELLS * Config::CELL_SIZE; // largeur externe cible
	const int innerMax = std::max(1, boxMaxWidth - (OVERHEAD_MESSAGE_PADDING * 2)); // largeur dispo pour le texte

	const int paddingX = std::max(1, ctx_.toWindowW(static_cast<float>(OVERHEAD_MESSAGE_PADDING)));
	const int paddingY = std::max(1, ctx_.toWindowH(static_cast<float>(OVERHEAD_MESSAGE_PADDING)));

	Texture textTexture = ctx_.createTextureFromText(ctx_.getDefaultFont(), msg, Config::WHITE, ctx_.toWindowW(static_cast<float>(innerMax)));

	int textWidth = 0;
	int textHeight = 0;
	textTexture.querySize(textWidth, textHeight);

	// Taille réelle du cadre (texte + padding de chaque côté)
	const int boxWidth  = textWidth  + (paddingX * 2);
	const int boxHeight = textHeight + (paddingY * 2);

	// Position du cadre (on centre sur le héros avec la largeur réelle du cadre)
	const int xBox = ctx_.toWindowX(static_cast<float>(hero_.getXScreen()) + (Config::CELL_SIZE / 2.0f)) - (boxWidth / 2);
	const int yBox = ctx_.toWindowY(static_cast<float>(hero_.getYScreen()) - (Config::CELL_SIZE / 4.0f)) - boxHeight;

	const SDL_Rect boxRect{xBox, yBox, boxWidth, boxHeight};
	ctx_.drawRectangle(boxRect, ctx_.getCurrentBoxColor());

	const SDL_Rect dstRect{xBox + paddingX, yBox + paddingY, textWidth, textHeight};
	ctx_.drawTexture(textTexture, dstRect);
}