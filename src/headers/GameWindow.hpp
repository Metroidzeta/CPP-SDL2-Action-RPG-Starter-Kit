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
#include <SDL2/SDL.h>

class GameWindow final {
private:
	std::string title_;
	int width_ = 0;
	int height_ = 0;

	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	SDL_Texture* worldTarget1_ = nullptr;
	SDL_Texture* worldTarget2_ = nullptr;
	bool fullscreenEnabled_ = false;
	bool vsyncEnabled_ = false;

	void destroyResources() noexcept;
	SDL_Rect getRenderViewport() const noexcept;

public:
	/** Constructeur **/
	GameWindow(const std::string& title, int width, int height, bool fullscreen = false, bool vsync = false);

	/** Destructeur **/
	~GameWindow() noexcept;

	/** Copie interdite **/
	GameWindow(const GameWindow&) = delete;
	GameWindow& operator=(const GameWindow&) = delete;

	/** Move interdit **/
	GameWindow(GameWindow&&) = delete;
	GameWindow& operator=(GameWindow&&) = delete;

	/** Getters **/
	const std::string& getTitle() const noexcept { return title_; }
	int                getWidth() const noexcept { return width_; }
	int               getHeight() const noexcept { return height_; }
	SDL_Window*       getWindow() const noexcept { return window_; }
	SDL_Renderer*   getRenderer() const noexcept { return renderer_; }
	bool           isFullscreen() const noexcept { return fullscreenEnabled_; }
	bool         isVSyncEnabled() const noexcept { return vsyncEnabled_; }
	int              getOffsetX() const noexcept;
	int              getOffsetY() const noexcept;
	float       getUniformScale() const noexcept;

	/** Setters **/
	void setTitle(const std::string& title);
	void setFullscreen(bool enabled);
	void setSize(int width, int height);

	/** Rendu **/
	void beginWorld1Render() noexcept;
	void flushWorld1Render() noexcept;
	void beginWorld2Render() noexcept;
	void flushWorld2Render() noexcept;

	void clear() noexcept;
	void present() noexcept;
};