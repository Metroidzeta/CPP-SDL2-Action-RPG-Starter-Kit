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
#include <unordered_map>
#include <SDL2/SDL.h>

#include "Controls.hpp"
#include "GameWindow.hpp"
#include "../resources/headers/Font.hpp"
#include "Hero.hpp"
#include "GameContext.hpp"
#include "../renderer/headers/Renderer.hpp"
#include "Camera.hpp"
#include "EventGroup.hpp"
#include "../events/Event.hpp"

/**
 * Classe principale de la boucle de rendu
 */
class GameLoop final {
private:
	static constexpr Uint64 ATTACK_NANOS_COOLDOWN = 667'000'000ULL; // 2/3 secondes
	static constexpr int FLASK_ANIMATION_FRAME_COUNT = 3;

	GameContext& gc_;
	GameWindow& gameWindow_;
	Renderer& renderer_;
	Controls& controls_;

	bool isRunning_ = false;
	bool wallsVisible_ = false, menuVisible_ = false;
	int flaskFrame_ = 0, messageCooldown_ = 0;
	Uint64 frames_ = 0ULL, lastAttackCooldown_ = 0ULL;
	double fpsResult_ = 0;
	// int damageVisible_ = 0;
	bool refreshNextFrame_ = false;
	EventGroup* currentsEvents_ = nullptr;

	Hero& hero_;
	Camera& camera_;
	std::unordered_map<std::string, Font>& fonts_;

	/** Méthodes privées **/
	void printFPS_Window();
	void refreshWindowDependentRendering();
	void renderImmediately();
	void executeEvent(const Event& ev);
	void updateCooldowns(Uint64 now) noexcept;
	bool updateHeroMovement();
	void updateUPS(Uint64 frameStartTime);
	void render();
	void updateFPS();

public:
	/** Constructeur **/
	GameLoop(GameContext& gc, GameWindow& gameWindow, Renderer& renderer, Controls& controls, std::unordered_map<std::string, Font>& fonts);

	/** Copie interdite **/
	GameLoop(const GameLoop&) = delete;
	GameLoop& operator=(const GameLoop&) = delete;

	/** Méthodes utilitaires **/
	void start();
};