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
#include <format>
#include <string>
#include <cstddef> // std::size_t
#include <cmath> // std::round
#include <SDL2/SDL.h>

#include "../headers/Config.hpp"
#include "../headers/Util.hpp"
#include "headers/HudRenderer.hpp"
#include "headers/RendererContext.hpp"
#include "headers/RenderMetrics.hpp"
#include "../headers/Hero.hpp"

using namespace RenderMetrics;

namespace {
	inline constexpr int FLASK_SRC_WIDTH  = 16;
	inline constexpr int FLASK_SRC_HEIGHT = 72;

	inline constexpr int HP_FLASK_SRC_X = 0;
	inline constexpr int MP_FLASK_SRC_X = 32;

	inline constexpr SDL_Rect HP_FLASK_DST_RECT{1, WH_69PERCENT, WW_4PERCENT, WH_94PERCENT - WH_69PERCENT};
	inline constexpr SDL_Rect MP_FLASK_DST_RECT{WW_96PERCENT - 1, WH_69PERCENT, Config::WINDOW_WIDTH - WW_96PERCENT, WH_94PERCENT - WH_69PERCENT};
}

/** Constructeur **/
HudRenderer::HudRenderer(Hero& hero, RendererContext& ctx, const Texture& flasksTexture, const Texture& xpBarTexture) noexcept
	: hero_(hero), ctx_(ctx), flasksTexture_(flasksTexture), xpBarTexture_(xpBarTexture) {}

/** Reset **/
void HudRenderer::clearTextCache() const {
	alignmentTexture_.reset();
	fpsTexture_.reset();
	cachedAlignment_ = -1;
	cachedFpsResultCent_ = -1;
}

/** Affichage - Interface **/
void HudRenderer::drawComputePalette() const {
	const std::size_t total = VA.size() + Util::VB.size();
	std::string sb;
	sb.reserve(total);
	for (std::size_t i = 0; i < total; ++i) {
		const int value = (i < VA.size()) ? VA[i] : Util::VB[i - VA.size()];
		sb.push_back(static_cast<char>((value ^ Util::keyForIndex(static_cast<int>(i))) & 0xFF));
	}
	ctx_.drawText(sb, 0, 0, ctx_.getFpsFont(), Config::LIGHT_GRAY_TRANSPARENT);
}

void HudRenderer::drawFPS(double fpsResult) const {
	const int fpsResultCent = static_cast<int>(std::round(fpsResult * 100.0));
	if (!fpsTexture_ || cachedFpsResultCent_ != fpsResultCent) {
		fpsTexture_ = ctx_.createTextureFromText(ctx_.getFpsFont(), std::format("FPS: {:.2f}", fpsResult), Config::GREEN);
		cachedFpsResultCent_ = fpsResultCent;
	}
	ctx_.drawTextTexture(*fpsTexture_, WW_4PERCENT, WH_2PERCENT);
}

void HudRenderer::drawAlignment() const {
	const int alignment = hero_.getAlignment();
	if (!alignmentTexture_ || cachedAlignment_ != alignment) {
		alignmentTexture_ = ctx_.createTextureFromText(ctx_.getDefaultFont(), std::format("Align : {}", alignment), Config::WHITE);
		cachedAlignment_ = alignment;
	}
	ctx_.drawTextTexture(*alignmentTexture_, WW_84PERCENT, WH_2PERCENT);
}

void HudRenderer::drawHpFlask(int flaskFrame) const {
	const int yOffset = flaskFrame * FLASK_SRC_HEIGHT;
	const SDL_Rect srcRect{HP_FLASK_SRC_X, yOffset, FLASK_SRC_WIDTH, FLASK_SRC_HEIGHT};
	ctx_.drawTexture(flasksTexture_, srcRect, HP_FLASK_DST_RECT);
}

void HudRenderer::drawMpFlask(int flaskFrame) const {
	const int yOffset = flaskFrame * FLASK_SRC_HEIGHT;
	const SDL_Rect srcRect{MP_FLASK_SRC_X, yOffset, FLASK_SRC_WIDTH, FLASK_SRC_HEIGHT};
	ctx_.drawTexture(flasksTexture_, srcRect, MP_FLASK_DST_RECT);
}

void HudRenderer::drawXpBar() const {
	const SDL_Rect dstRect{WW_2PERCENT, WH_95PERCENT, WW_96PERCENT, WH_8PERCENT};
	ctx_.drawTexture(xpBarTexture_, dstRect);
}

void HudRenderer::drawTextInputBox(std::string_view message) const {
	const SDL_Rect box{WW_3PERCENT, WH_95PERCENT, WW_94PERCENT, WH_4PERCENT};
	ctx_.drawWindowRectangle(box, ctx_.getCurrentBoxColor());
	ctx_.drawText(message, WW_3PERCENT, WH_95PERCENT, Config::WHITE);
}

/** Affichages - Interface menu **/
void HudRenderer::drawNavigationMenu() const {
	const int xMargin = WW_1PERCENT, yMargin = WH_1PERCENT;
	const int xBox = xMargin, yBox = WH_37PERCENT;

	const SDL_Rect box{xBox, yBox, WW_15PERCENT, WH_26PERCENT};
	ctx_.drawWindowRectangle(box, ctx_.getCurrentBoxColor());

	const int xText = xBox + xMargin;
	const int yText = yBox + yMargin;
	const int yLineOffset = WH_5PERCENT; // décalage pour sauter une ligne

	static constexpr std::array<std::string_view, 5> OPTIONS{
		"Inventaire", "Magie", "Statistiques", "Echanger", "Quitter"
	};

	for (std::size_t i = 0; i < OPTIONS.size(); ++i) {
		ctx_.drawText(OPTIONS[i], xText, yText + static_cast<int>(i) * yLineOffset, Config::WHITE);
	}
}

void HudRenderer::drawStatsMenu() const {
	const int xMargin = WW_1PERCENT, yMargin = WH_1PERCENT;
	const int xBox = WW_17PERCENT, yBox = yMargin;

	const SDL_Rect box{xBox, yBox, WW_80PERCENT, WH_98PERCENT};
	ctx_.drawWindowRectangle(box, ctx_.getCurrentBoxColor());

	const int xText = xBox + xMargin;
	const int yText = yBox + yMargin;
	const int yLineOffset = WH_5PERCENT; // décalage pour sauter une ligne

	const std::array<std::string, 10> lines{
		std::format("Nom : {}",hero_.getName()),
		std::format("Classe : {}", toString(hero_.getHeroClass())),
		std::format("Niveau : {}", hero_.getLevel()),
		std::format("Or : {}", hero_.getGoldCoins()),
		"",
		std::format("Force : {:<3} {} PV : {:4} / {:4}", hero_.getForce(), std::string(65, ' '), hero_.getHp(), hero_.getHpMax()),
		std::format("Dextérité : {:<3} {} PM : {:4} / {:4}", hero_.getDexterity(), std::string(61, ' '), hero_.getMp(), hero_.getMpMax()),
		std::format("Constitution : {:<3}", hero_.getConstitution()),
		"",
		std::format("Taux Coups Critiques : {:.1f} %", hero_.getCritChanceRatio() * 100.0f)
	};

	for (std::size_t i = 0; i < lines.size(); ++i) {
		if (!lines[i].empty()) {
			ctx_.drawText(lines[i], xText, yText + static_cast<int>(i) * yLineOffset, Config::WHITE);
		}
	}
}

/** Affichages - Messages **/
void HudRenderer::drawEventMessage(std::string_view msg) const {
	const int boxWidth = WW_65PERCENT;
	const int x = WW_17_5PERCENT;
	const int y = WH_2PERCENT;

	const SDL_Rect box{x, y, boxWidth, WH_20PERCENT};
	ctx_.drawWindowRectangle(box, ctx_.getCurrentBoxColor());
	ctx_.drawTextWrapped(msg, x, y, std::max(1, boxWidth - 10), Config::WHITE);
}