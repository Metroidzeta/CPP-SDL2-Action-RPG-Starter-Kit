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
#include <format>
#include <algorithm> // std::max

#include "headers/RendererContext.hpp"
#include "../resources/headers/Surface.hpp"

/** Constructeur **/
RendererContext::RendererContext(SDL_Renderer* renderer, const Font& defaultFont, const Font& fpsFont, float scaleX, float scaleY)
	: renderer_(renderer), defaultFont_(&defaultFont), fpsFont_(&fpsFont), scaleX_(scaleX), scaleY_(scaleY) {
	if (!renderer_) throw std::invalid_argument("RendererContext: renderer NULL");
	if (SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND) != 0) {
		throw std::runtime_error(std::format("Echec SDL_SetRenderDrawBlendMode : {}", SDL_GetError()));
	}
}

/** Méthodes privées **/
void RendererContext::changeRenderDrawColor(SDL_Color color) const {
	if (SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a) != 0) {
		throw std::runtime_error(std::format("Echec SDL_SetRenderDrawColor : {}", SDL_GetError()));
	}
}

void RendererContext::renderTextTexture(const Texture& texture, int x, int y) const {
	SDL_Rect dstRect{toWindowX(static_cast<float>(x)), toWindowY(static_cast<float>(y)), 0, 0};
	texture.querySize(dstRect.w, dstRect.h);
	drawTexture(texture, dstRect);
}

/** Méthodes publiques - Méthodes de création **/
Texture RendererContext::createTextureFromText(const Font& font, std::string_view text, SDL_Color color, int maxWidth) const {
	if (text.empty()) throw std::invalid_argument("Texte vide");

	Surface surface = maxWidth > 0
		? Surface(font, text, color, maxWidth)
		: Surface(font, text, color);

	return Texture(renderer_, surface);
}

SDL_Rect RendererContext::toWindowRect(const SDL_Rect& rect) const noexcept {
	return SDL_Rect{
		toWindowX(static_cast<float>(rect.x)),
		toWindowY(static_cast<float>(rect.y)),
		toWindowW(static_cast<float>(rect.w)),
		toWindowH(static_cast<float>(rect.h))
	};
}

/** Méthodes de rendu **/
void RendererContext::drawRectangle(const SDL_Rect& rect, SDL_Color color) const {
	changeRenderDrawColor(color);
	if (SDL_RenderFillRect(renderer_, &rect) != 0) {
		throw std::runtime_error(std::format("Echec dessin rectangle avec SDL_RenderFillRect : {}", SDL_GetError()));
	}
}

void RendererContext::drawWindowRectangle(const SDL_Rect& rect, SDL_Color color) const {
	drawRectangle(toWindowRect(rect), color);
}

void RendererContext::drawTexture(const Texture& texture, const SDL_Rect& dstRect) const {
	if (SDL_RenderCopy(renderer_, texture.get(), nullptr, &dstRect) != 0) {
		throw std::runtime_error(std::format("Echec affichage texture sans source : {}", SDL_GetError()));
	}
}

void RendererContext::drawTexture(const Texture& texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect) const {
	if (SDL_RenderCopy(renderer_, texture.get(), &srcRect, &dstRect) != 0) {
		throw std::runtime_error(std::format("Echec affichage texture avec source : {}", SDL_GetError()));
	}
}

void RendererContext::drawText(std::string_view text, int x, int y, SDL_Color color) const {
	drawText(text, x, y, *defaultFont_, color);
}

void RendererContext::drawText(std::string_view text, int x, int y, const Font& font, SDL_Color color) const {
	if (text.empty()) return;
	Texture texture = createTextureFromText(font, text, color);
	renderTextTexture(texture, x, y);
}

void RendererContext::drawTextTexture(const Texture& texture, int x, int y) const {
	renderTextTexture(texture, x, y);
}

void RendererContext::drawTextWrapped(std::string_view text, int x, int y, int maxWidth, SDL_Color color) const {
	drawTextWrapped(text, x, y, maxWidth, *defaultFont_, color);
}

void RendererContext::drawTextWrapped(std::string_view text, int x, int y, int maxWidth, const Font& font, SDL_Color color) const {
	if (text.empty()) return;
	if (maxWidth <= 0) throw std::invalid_argument("maxWidth <= 0");

	const int nativeMaxWidth = std::max(1, toWindowW(static_cast<float>(maxWidth)));
	Texture texture = createTextureFromText(font, text, color, nativeMaxWidth);
	renderTextTexture(texture, x, y);
}

/** Autres méthodes **/
void RendererContext::nextBoxColor() noexcept { boxColorIndex_ = (boxColorIndex_ + 1) % BOX_COLORS.size(); }