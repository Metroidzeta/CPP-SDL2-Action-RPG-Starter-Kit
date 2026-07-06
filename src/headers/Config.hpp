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

#include <SDL2/SDL.h>

/**
 * Classe de configuration globale
 */
class Config final {
private:
	Config() = delete; // Empêche l'instanciation
	~Config() = delete; // Empêche la destruction
	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

public:
	/** Titre de la fenêtre **/
	static constexpr const char* WINDOW_TITLE = "C++/SDL2 Action RPG Starter Kit";

	/** Paramètres de base **/
	static constexpr int WINDOW_WIDTH  = 1280, WINDOW_HEIGHT = 960; // par défaut : 1280 * 960
	static constexpr int UPS = 30; // par défaut : 30
	static constexpr int FPS = 60; // par défaut : 60
	static constexpr int LEVEL_MAX = 80; // par défaut : 80

	/** Constantes dérivées — NE PAS LES MODIFIER /!\ **/
	static constexpr int CELL_SIZE = (WINDOW_HEIGHT / 20) - ((WINDOW_HEIGHT / 20) % 4); // par défaut : 48
	static constexpr int HERO_MOVE_STEP = CELL_SIZE / 4;
	static constexpr int WINDOW_WIDTH_CELLS = (WINDOW_WIDTH + CELL_SIZE - 1) / CELL_SIZE;
	static constexpr int WINDOW_HEIGHT_CELLS = (WINDOW_HEIGHT + CELL_SIZE - 1) / CELL_SIZE;

	/** Couleurs semi-transparentes RVB **/
	static constexpr SDL_Color WHITE                  {255, 255, 255, 255};
	static constexpr SDL_Color BLACK                  {  0,   0,   0, 255};
	static constexpr SDL_Color GREEN                  {  0, 255,   0, 255};
	static constexpr SDL_Color DARK_BLUE_TRANSPARENT  {  0,   0, 189, 180};
	static constexpr SDL_Color DARK_GREEN_TRANSPARENT {  0, 100,   0, 180};
	static constexpr SDL_Color BURGUNDY_TRANSPARENT   {109,   7,  26, 180};
	static constexpr SDL_Color DARK_GOLD_TRANSPARENT  {181, 148,  16, 180};
	static constexpr SDL_Color LIGHT_GRAY_TRANSPARENT {180, 190, 200,  48};
	static constexpr SDL_Color DARK_GRAY_TRANSPARENT  { 58,  58,  58, 180};
	static constexpr SDL_Color PURPLE_TRANSPARENT     {143,   0, 255, 128};

	/** Dossiers **/
	static constexpr const char* UI_ASSETS_DIRECTORY = "img";
	static constexpr const char* FONTS_DIRECTORY = "fonts";
	static constexpr const char* CHIPSETS_DIRECTORY = "img";
	static constexpr const char* SKINS_DIRECTORY = "img";
	static constexpr const char* SOUND_EFFECTS_DIRECTORY = "sfx";
	static constexpr const char* MUSIC_DIRECTORY = "music";
	static constexpr const char* GAME_MAP_DIRECTORY = "maps";

	/** Limites **/
	static constexpr int MSG_SIZE_MAX = 45; // par défaut : 45
	static constexpr bool DEBUG_MODE = false; // par défaut : false
};