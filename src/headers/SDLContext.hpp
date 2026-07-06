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

#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class SDLContext final {
public:
	/** Constructeur **/
	SDLContext() {
		#ifdef _WIN32
			SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
		#endif

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // Pour ne pas avoir un jeu flou

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
			throw std::runtime_error(std::string("Echec SDL_Init : ") + SDL_GetError());
		}

		constexpr int imageFlags = IMG_INIT_PNG;

		if ((IMG_Init(imageFlags) & imageFlags) != imageFlags) {
			const std::string imgError = IMG_GetError();
			IMG_Quit();
			SDL_Quit();
			throw std::runtime_error("Echec IMG_Init : " + imgError);
		}

		if (TTF_Init() != 0) {
			const std::string ttfError = TTF_GetError();
			IMG_Quit();
			SDL_Quit();
			throw std::runtime_error("Echec TTF_Init : " + ttfError);
		}

		constexpr int mixerFlags = MIX_INIT_OGG;

		if ((Mix_Init(mixerFlags) & mixerFlags) != mixerFlags) {
			const std::string mixError = Mix_GetError();
			Mix_Quit();
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			throw std::runtime_error("Echec Mix_Init : " + mixError);
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			const std::string mixError = Mix_GetError();
			Mix_Quit();
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			throw std::runtime_error("Echec Mix_OpenAudio : " + mixError);
		}
	}

	/** Destructeur **/
	~SDLContext() noexcept {
		Mix_CloseAudio();
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	/** Copie interdite **/
	SDLContext(const SDLContext&) = delete;
	SDLContext& operator=(const SDLContext&) = delete;

	/** Move interdit **/
	SDLContext(SDLContext&&) = delete;
	SDLContext& operator=(SDLContext&&) = delete;
};