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
#include <cstddef>
#include <string_view>
#include <SDL2/SDL.h>
#include <cmath> // std::lround

#include "../../headers/Config.hpp"
#include "../../resources/headers/Font.hpp"
#include "../../resources/headers/Texture.hpp"

class RendererContext final {
private:
	inline static constexpr std::array<SDL_Color, 5> BOX_COLORS{
		Config::DARK_BLUE_TRANSPARENT,
		Config::DARK_GREEN_TRANSPARENT,
		Config::BURGUNDY_TRANSPARENT,
		Config::DARK_GOLD_TRANSPARENT,
		Config::DARK_GRAY_TRANSPARENT
	};

	int offsetX_ = 0;
	int offsetY_ = 0;

	SDL_Renderer* renderer_ = nullptr; // non-owning
	const Font* defaultFont_ = nullptr;
	const Font* fpsFont_ = nullptr;
	float scaleX_ = 1.0f, scaleY_ = 1.0f;
	std::size_t boxColorIndex_ = 0;

	/** Méthodes privées **/
	void changeRenderDrawColor(SDL_Color color) const;
	void renderTextTexture(const Texture& texture, int x, int y) const;

public:
	/** Constructeur **/
	RendererContext(SDL_Renderer* renderer, const Font& defaultFont, const Font& fpsFont, float scaleX, float scaleY);

	/** Copie interdite **/
	RendererContext(const RendererContext&) = delete;
	RendererContext& operator=(const RendererContext&) = delete;

	/** Move interdit **/
	RendererContext(RendererContext&&) = delete;
	RendererContext& operator=(RendererContext&&) = delete;

	/** Getters **/
	SDL_Renderer* getRenderer()        const noexcept { return renderer_; }
	const Font&   getDefaultFont()     const noexcept { return *defaultFont_; }
	const Font&   getFpsFont()         const noexcept { return *fpsFont_; }
	float         getWindowScaleX()    const noexcept { return scaleX_; }
	float         getWindowScaleY()    const noexcept { return scaleY_; }
	SDL_Color     getCurrentBoxColor() const noexcept { return BOX_COLORS[boxColorIndex_]; }

	/** Setters **/
	void setFonts(const Font& defaultFont, const Font& fpsFont) noexcept { defaultFont_ = &defaultFont; fpsFont_ = &fpsFont; }
	void setWindowTransform(float scaleX, float scaleY, int offsetX, int offsetY) noexcept { scaleX_ = scaleX; scaleY_ = scaleY; offsetX_ = offsetX; offsetY_ = offsetY; }

	/** Méthodes de création **/
	Texture createTextureFromText(const Font& font, std::string_view text, SDL_Color color, int maxWidth = 0) const;

	/** Conversion coordonnées logiques -> fenêtre réelle **/
	int toWindowX(float x) const noexcept { return offsetX_ + static_cast<int>(std::lround(x * scaleX_)); }
	int toWindowY(float y) const noexcept { return offsetY_ + static_cast<int>(std::lround(y * scaleY_)); }
	int toWindowW(float w) const noexcept { return static_cast<int>(std::lround(w * scaleX_)); }
	int toWindowH(float h) const noexcept { return static_cast<int>(std::lround(h * scaleY_)); }
	SDL_Rect toWindowRect(const SDL_Rect& rect) const noexcept;

	/** Méthodes de rendu **/
	void drawRectangle(const SDL_Rect& rect, SDL_Color color = Config::WHITE) const;
	void drawWindowRectangle(const SDL_Rect& rect, SDL_Color color = Config::WHITE) const;
	void drawTexture(const Texture& texture, const SDL_Rect& dstRect) const;
	void drawTexture(const Texture& texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect) const;
	void drawTextTexture(const Texture& texture, int x, int y) const;
	void drawText(std::string_view text, int x, int y, SDL_Color color = Config::WHITE) const;
	void drawText(std::string_view text, int x, int y, const Font& font, SDL_Color color = Config::WHITE) const;
	void drawTextWrapped(std::string_view text, int x, int y, int maxWidth, SDL_Color color = Config::WHITE) const;
	void drawTextWrapped(std::string_view text, int x, int y, int maxWidth, const Font& font, SDL_Color color = Config::WHITE) const;

	/** Autres méthodes **/
	void nextBoxColor() noexcept;
};