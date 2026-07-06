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
#include "headers/GameWindow.hpp"

#include <stdexcept>
#include <algorithm>
#include <cmath> // std::min

namespace {
	float getDpiScale() noexcept {
#ifdef _WIN32 // Windows only
		float ddpi = 96.0f;
		if (SDL_GetDisplayDPI(0, &ddpi, nullptr, nullptr) == 0 && ddpi > 0.0f) return ddpi / 96.0f;
#endif
		return 1.0f;
	}

	int toPhysicalSize(int logicalSize) noexcept {
		return static_cast<int>(logicalSize * getDpiScale());
	}
}

/** Constructeur **/
GameWindow::GameWindow(const std::string& title, int width, int height, bool fullscreen, bool vsync)
	: title_(title), width_(width), height_(height), fullscreenEnabled_(fullscreen), vsyncEnabled_(vsync) {
	if (title.empty()) throw std::invalid_argument("Titre de fenêtre vide");
	if (width <= 0 || height <= 0) throw std::invalid_argument("Dimensions de fenêtre invalides");

	Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
	if (fullscreen) { windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }

	const int physicalWidth  = toPhysicalSize(width_), physicalHeight = toPhysicalSize(height_);
	window_ = SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, physicalWidth, physicalHeight, windowFlags);
	if (!window_) throw std::runtime_error(std::string("Echec SDL_CreateWindow: ") + SDL_GetError());

	Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	if (vsyncEnabled_) { rendererFlags |= SDL_RENDERER_PRESENTVSYNC; }

	renderer_ = SDL_CreateRenderer(window_, -1, rendererFlags);
	if (!renderer_) {
		const std::string sdlError = SDL_GetError();
		SDL_DestroyWindow(window_);
		window_ = nullptr;
		throw std::runtime_error("Echec SDL_CreateRenderer : " + sdlError);
	}

	worldTarget1_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width_, height_);
	if (!worldTarget1_) {
		const std::string sdlError = SDL_GetError();
		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
		renderer_ = nullptr;
		window_ = nullptr;
		throw std::runtime_error("Echec SDL_CreateTexture worldTarget1 : " + sdlError);
	}

	worldTarget2_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width_, height_);
	if (!worldTarget2_) {
		const std::string sdlError = SDL_GetError();
		SDL_DestroyTexture(worldTarget1_);
		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
		worldTarget1_ = nullptr;
		renderer_ = nullptr;
		window_ = nullptr;
		throw std::runtime_error("Echec SDL_CreateTexture worldTarget2 : " + sdlError);
	}

	if (SDL_SetTextureScaleMode(worldTarget1_, SDL_ScaleModeNearest) != 0) {
		const std::string sdlError = SDL_GetError();
		destroyResources();
		throw std::runtime_error("Echec SDL_SetTextureScaleMode worldTarget1 : " + sdlError);
	}

	if (SDL_SetTextureScaleMode(worldTarget2_, SDL_ScaleModeNearest) != 0) {
		const std::string sdlError = SDL_GetError();
		destroyResources();
		throw std::runtime_error("Echec SDL_SetTextureScaleMode worldTarget2 : " + sdlError);
	}

	if (SDL_SetTextureBlendMode(worldTarget2_, SDL_BLENDMODE_BLEND) != 0) {
		const std::string sdlError = SDL_GetError();
		destroyResources();
		throw std::runtime_error("Echec SDL_SetTextureBlendMode worldTarget2 : " + sdlError);
	}

	// Blending alpha par défaut
	if (SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND) != 0) {
		const std::string sdlError = SDL_GetError();
		destroyResources();
		throw std::runtime_error("Echec SDL_SetRenderDrawBlendMode : " + sdlError);
	}
}

/** Destructeur **/
GameWindow::~GameWindow() noexcept {
	SDL_DestroyTexture(worldTarget1_);
	SDL_DestroyTexture(worldTarget2_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

void GameWindow::destroyResources() noexcept {
	SDL_DestroyTexture(worldTarget2_);
	SDL_DestroyTexture(worldTarget1_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);

	worldTarget2_ = nullptr;
	worldTarget1_ = nullptr;
	renderer_ = nullptr;
	window_ = nullptr;
}

/** Getters **/
SDL_Rect GameWindow::getRenderViewport() const noexcept {
	int outputWidth = 0, outputHeight = 0;
	if (!renderer_ || SDL_GetRendererOutputSize(renderer_, &outputWidth, &outputHeight) != 0) {
		return SDL_Rect{0, 0, width_, height_};
	}

	const float scale = std::min(
		static_cast<float>(outputWidth) / static_cast<float>(width_),
		static_cast<float>(outputHeight) / static_cast<float>(height_)
	);

	const int renderWidth = static_cast<int>(std::lround(width_ * scale));
	const int renderHeight = static_cast<int>(std::lround(height_ * scale));
	const int offsetX = (outputWidth - renderWidth) / 2;
	const int offsetY = (outputHeight - renderHeight) / 2;

	return SDL_Rect{offsetX, offsetY, renderWidth, renderHeight};
}

float GameWindow::getUniformScale() const noexcept {
	const SDL_Rect viewport = getRenderViewport();
	return static_cast<float>(viewport.w) / static_cast<float>(width_);
}

int GameWindow::getOffsetX() const noexcept { return getRenderViewport().x; }
int GameWindow::getOffsetY() const noexcept { return getRenderViewport().y; }

/** Setters **/
void GameWindow::setTitle(const std::string& title) {
	if (title.empty()) throw std::invalid_argument("Titre de fenêtre vide");
	title_ = title;
	if (window_) SDL_SetWindowTitle(window_, title_.c_str());
}

void GameWindow::setFullscreen(bool enabled) {
	if (!window_) return;
	const Uint32 flags = enabled ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
	if (SDL_SetWindowFullscreen(window_, flags) != 0) {
		throw std::runtime_error(std::string("Echec SDL_SetWindowFullscreen : ") + SDL_GetError());
	}
	fullscreenEnabled_ = enabled;
}

void GameWindow::setSize(int width, int height) {
	if (width <= 0 || height <= 0) throw std::invalid_argument("Dimensions de fenêtre invalides");
	width_ = width; height_ = height;
	if (window_) {
		const int physicalWidth  = toPhysicalSize(width_), physicalHeight = toPhysicalSize(height_);
		SDL_SetWindowSize(window_, physicalWidth, physicalHeight);
	}
}

/** Rendu **/
void GameWindow::beginWorld1Render() noexcept {
	if (!renderer_ || !worldTarget1_) return;
	SDL_SetRenderTarget(renderer_, worldTarget1_); // On dessine sur worldTarget1
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255); // noir par défaut
	SDL_RenderClear(renderer_);
}

void GameWindow::flushWorld1Render() noexcept {
	if (!renderer_ || !worldTarget1_) return;
	SDL_SetRenderTarget(renderer_, nullptr); // On dessine sur la fenêtre directement
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255); // noir par défaut
	SDL_RenderClear(renderer_);

	const SDL_Rect dst = getRenderViewport();
	SDL_RenderCopy(renderer_, worldTarget1_, nullptr, &dst); // Rendu de worldTarget1 dans la fenêtre
}

void GameWindow::beginWorld2Render() noexcept {
	if (!renderer_ || !worldTarget2_) return;
	SDL_SetRenderTarget(renderer_, worldTarget2_); // On dessine sur worldTarget2
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0); // noir transparent par défaut
	SDL_RenderClear(renderer_);
}

void GameWindow::flushWorld2Render() noexcept {
	if (!renderer_ || !worldTarget2_) return;
	SDL_SetRenderTarget(renderer_, nullptr); // On dessine sur la fenêtre directement

	const SDL_Rect dst = getRenderViewport();
	SDL_RenderCopy(renderer_, worldTarget2_, nullptr, &dst); // Rendu de worldTarget2 dans la fenêtre
}

void GameWindow::clear() noexcept {
	if (!renderer_) return;
	SDL_RenderClear(renderer_);
}

void GameWindow::present() noexcept {
	if (!renderer_) return;
	SDL_RenderPresent(renderer_);
}