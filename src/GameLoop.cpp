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
#include <thread> // std::this_thread
#include <iostream>
#include <sstream> // std::ostringstream
#include <iomanip> // std::setprecision
#include <algorithm> // pour std::clamp

#include "loaders/headers/LoadFonts.hpp"
#include "events/Event_MSG.hpp"
#include "events/Event_TP.hpp"
#include "headers/Config.hpp"
#include "headers/Util.hpp"
#include "headers/GameLoop.hpp"

namespace {
	Uint64 nowNanos() noexcept {
		static const long double invFreq = 1'000'000'000.0L / static_cast<long double>(SDL_GetPerformanceFrequency());
		return static_cast<Uint64>(SDL_GetPerformanceCounter() * invFreq);
	}
}

GameLoop::GameLoop(GameContext& gc, GameWindow& gameWindow, Renderer& renderer, Controls& controls, std::unordered_map<std::string, Font>& fonts)
	: gc_(gc), gameWindow_(gameWindow), renderer_(renderer), controls_(controls),
	hero_(gc_.getHero()), camera_(gc_.getCamera()), fonts_(fonts) {}

void GameLoop::printFPS_Window() {
	std::ostringstream oss;
	oss << Config::WINDOW_TITLE << " | FPS : " << std::fixed << std::setprecision(2) << fpsResult_;
	gameWindow_.setTitle(oss.str());
}

void GameLoop::refreshWindowDependentRendering() {
	const float scale = gameWindow_.getUniformScale();
	fonts_ = LoadFonts::get(scale);
	renderer_.setWindowTransform(scale, scale, gameWindow_.getOffsetX(), gameWindow_.getOffsetY());
	renderer_.setFonts(fonts_.at("Base"), fonts_.at("FPS"));
	renderer_.clearTextCache();
}

void GameLoop::renderImmediately() {
	gc_.resetCamera();
	//dessiner_.effacerEcran();
	camera_.interpolate(1.0); // interpolation complète
	render(); // afficher directement la nouvelle carte
	refreshNextFrame_ = true;
}

void GameLoop::executeEvent(const Event& ev) {
	hero_.setIsBlocked(true);
	hero_.setIsInEvent(true);
	ev.execute(gc_);
	bool isEvent_MSG = false;
	if (dynamic_cast<const Event_MSG*>(&ev)) { gc_.clearMessage(); isEvent_MSG = true; }
	if (dynamic_cast<const Event_TP*>(&ev)) {
		std::cout << "Teleportation de " << hero_.getName() << " vers " << hero_.getCurrentGameMap().getName() << " " << hero_.getPositionCellsString() << '\n';
		renderImmediately();
	}

	if (!isEvent_MSG) {
		gc_.incrementNbEventPass();
		hero_.setIsInEvent(false);
	}
}

void GameLoop::updateCooldowns(Uint64 now) noexcept {
	if (!hero_.canSwordAttack() && (now - lastAttackCooldown_) > ATTACK_NANOS_COOLDOWN) hero_.setCanSwordAttack(true);
}

bool GameLoop::updateHeroMovement() {
	// --- Déplacements avec flèches ---
	const bool arrowsPressed = controls_.UP() || controls_.DOWN() || controls_.LEFT() || controls_.RIGHT();
	bool moving = false;
	if (arrowsPressed && !hero_.isBlocked()) {
		if (controls_.UP()    && !controls_.DOWN())  moving = hero_.move(Directions::UP)    || moving; // Ou moving car les autres directions peuvent fausser le résultat
		if (controls_.DOWN()  && !controls_.UP())    moving = hero_.move(Directions::DOWN)  || moving;
		if (controls_.LEFT()  && !controls_.RIGHT()) moving = hero_.move(Directions::LEFT)  || moving;
		if (controls_.RIGHT() && !controls_.LEFT())  moving = hero_.move(Directions::RIGHT) || moving;
	} else {
		hero_.setMoveFrame(7);
	}
	return moving;
}

void GameLoop::updateUPS(Uint64 frameStartTime) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_DISPLAY_CHANGED) {
				gameWindow_.setSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
				refreshWindowDependentRendering();
			}
		}
		if (event.type == SDL_QUIT) { isRunning_ = false; return; }
		controls_.handleEvent(event);
	}
	// --- Déplacements avec flèches ---
	const bool moving = updateHeroMovement();
	camera_.update();
	if (moving && !hero_.isBlocked() && !currentsEvents_) {
		currentsEvents_ = hero_.getCurrentGameMap().findCollidingEvents(hero_.getHitbox());
	}

	if (currentsEvents_ && !hero_.isInEvent()) {
		const Event* ev = currentsEvents_->getEventIfExists(0, gc_.getNbEventPass());
		if (ev) executeEvent(*ev);
	}

	if (!hero_.isWriting()) {
		if (controls_.A()) {
			wallsVisible_ = !wallsVisible_;
			if (Config::DEBUG_MODE) std::cout << (wallsVisible_ ? "Activation" : "Desactivation")  << " de l'affichage des murs!\n";
			controls_.consume(Controls::GameKey::A);
		}

		if (controls_.B()) {
			gameWindow_.setFullscreen(!gameWindow_.isFullscreen());
			refreshWindowDependentRendering();
			controls_.consume(Controls::GameKey::B);
		}

		if (controls_.Q()) {
			isRunning_ = false;
			controls_.consume(Controls::GameKey::Q);
		}

		if (!hero_.isBlocked() && controls_.S()) {
			if (hero_.canSwordAttack()) {
				gc_.getAudioManager().playSound("Blow1.wav");
				hero_.updateSwordHitBox();

				hero_.setSwordAttack(true);
				hero_.setCanSwordAttack(false);
				lastAttackCooldown_ = frameStartTime;
			}
			controls_.consume(Controls::GameKey::S);
		}
	}

	if (controls_.BACK_SPACE()) {
		if (hero_.isWriting()) gc_.deleteLastCharacterMessage();
		controls_.consume(Controls::GameKey::BACK_SPACE);
	}

	if (controls_.ENTER()) {
		if (!hero_.isBlocked()) {
			bool isWriting = hero_.isWriting();
			hero_.setIsWriting(!isWriting);
			if (isWriting && !gc_.getMessage().empty()) {
				messageCooldown_ = 0;
				hero_.setHasOverheadMessage(true);
				std::cout << hero_.getName() << " : " << Util::u32_to_u8(gc_.getMessage()) << '\n';
				//addMessageHistoric();
				gc_.saveMessage();
				gc_.clearMessage();
			}
		}
		controls_.consume(Controls::GameKey::ENTER);
	}

	if (controls_.SPACE()) {
		if (hero_.isInEvent() && currentsEvents_ && dynamic_cast<const Event_MSG*>(currentsEvents_->getEventIfExists(0, gc_.getNbEventPass()))) {
			gc_.incrementNbEventPass();
			hero_.setIsInEvent(false);
		}
		controls_.consume(Controls::GameKey::SPACE);
	}

	if (controls_.ESCAPE()) {
		if (!currentsEvents_) {
			//afficherRecap = 0;
			menuVisible_ = !menuVisible_;
			if (menuVisible_) {
				hero_.setIsWriting(false);
				gc_.clearMessage();
				hero_.setMoveFrame(7);
			}
			hero_.setIsBlocked(!hero_.isBlocked());
			controls_.consume(Controls::GameKey::ESCAPE);
		}
	}

	if (!hero_.isBlocked()) {
		if (controls_.F1()) {
			controls_.consume(Controls::GameKey::F1);
		}

		if (controls_.F3()) {
			hero_.setIsWriting(true);
			gc_.clearMessage();
			gc_.restoreLastMessage();
			controls_.consume(Controls::GameKey::F3);
		}
	}

	if (controls_.F5()) {
		renderer_.getContext().nextBoxColor();
		controls_.consume(Controls::GameKey::F5);
	}

	if (currentsEvents_ && currentsEvents_->isFinished(0, gc_.getNbEventPass())) {
		gc_.resetNbEventPass();
		currentsEvents_ = nullptr;
		hero_.setIsBlocked(false);
	}
}

void GameLoop::render() {
	// 1. Monde pixel-art sur texture
	gameWindow_.beginWorld1Render();
	renderer_.drawBlackBackground();
	renderer_.updateMapDisplayBounds();
	renderer_.drawLayer(0); // couche 0 chipset
	renderer_.drawLayer(1); // couche 1 chipset
	if (wallsVisible_) renderer_.drawWalls();
	renderer_.drawHero();
	gameWindow_.flushWorld1Render();

	// Afficher nom du héros dans la fenêtre directement
	renderer_.drawHeroName();

	// 2. Overlay monde/HUD images sur texture
	gameWindow_.beginWorld2Render();
	renderer_.drawLayer(2); // couche 2 chipset
	if (hero_.swordAttack()) {
		renderer_.drawHeroSwordHitbox();
		hero_.setSwordAttack(false);
	}

	renderer_.drawHpFlask(flaskFrame_);
	renderer_.drawMpFlask(flaskFrame_);
	renderer_.drawXpBar();

	gameWindow_.flushWorld2Render();

	// 3. UI directe fenêtre
	if (hero_.isWriting()) renderer_.drawTextInputBox(Util::u32_to_u8(gc_.getMessage()));
	if (hero_.hasOverheadMessage()) renderer_.drawHeroOverheadMessage(Util::u32_to_u8(gc_.getLastMessage()));
	if (hero_.isInEvent() && currentsEvents_) {
		const Event* ev = currentsEvents_->getEventIfExists(0, gc_.getNbEventPass());
		if (const auto* ev_msg = dynamic_cast<const Event_MSG*>(ev)) {
			renderer_.drawEventMessage(ev_msg->getMessage());
		}
	}

	if (menuVisible_) {
		renderer_.drawNavigationMenu(); // affiche menu de navigation
		renderer_.drawStatsMenu(); // affiche sous-menu: statistiques
	}
	renderer_.drawAlignment();
	renderer_.drawFPS(fpsResult_);
	if (refreshNextFrame_) {
		renderer_.drawComputePalette();
		refreshNextFrame_ = false;
	}

	gameWindow_.present(); // mettre à jour l'écran (afficher le rendu de la frame)
}

void GameLoop::updateFPS() {
	render();
	++frames_;
}

/** Boucle de jeu **/
void GameLoop::start() {
	// --- Constantes temporelles ---
	static constexpr Uint64 NANOS_PER_RENDER     = 1'000'000'000ULL / Config::FPS;
	static constexpr Uint64 NANOS_PER_TICK       = 1'000'000'000ULL / Config::UPS;
	static constexpr Uint64 NANOS_588MS          = 588'000'000ULL;
	static constexpr Uint64 NANOS_1SEC           = 1'000'000'000ULL;  // 1 seconde
	static constexpr Uint64 NANOS_1MIN           = 60'000'000'000ULL; // 1 minute
	static constexpr Uint64 MAX_FRAME_SKIP_NANOS = 5'000'000'000ULL;  // 5 secondes
	static constexpr int    NO_DELAYS_PER_YIELD  = 16;

	// --- Variables de synchronisation ---
	Uint64 now = nowNanos();
	Uint64 nextRender = now, nextTick = now;

	// --- Timers secondaires ---
	Uint64 lastFlaskTime = now, lastMinute = now, lastSecond = now;
	Uint64 overSleepNanos = 0ULL;
	int noDelays = 0;

	// --- Initialisation ---
	Uint64 lastFrameCount = 0ULL;
	gc_.getAudioManager().playMusic(hero_.getCurrentGameMap().getMusic());
	isRunning_ = true;

	// --- Boucle principale ---
	while (isRunning_) {
		now = nowNanos();

		// --- Détection d'une longue pause (veille, freeze, etc.) ---
		if (static_cast<Sint64>(now - nextTick) > static_cast<Sint64>(MAX_FRAME_SKIP_NANOS)) {
			nextTick = now + NANOS_PER_TICK;
			nextRender = now + NANOS_PER_RENDER;
			overSleepNanos = 0ULL;
			noDelays = 0;
		}

		// --- Logique globale ---
		updateCooldowns(now);
		//updateTimers(now);

		// --- Animation fioles ---
		if (now - lastFlaskTime >= NANOS_588MS) {
			flaskFrame_ = (flaskFrame_ + 1) % FLASK_ANIMATION_FRAME_COUNT;
			lastFlaskTime += NANOS_588MS;
		}

		// --- Chaque seconde ---
		if (now - lastSecond >= NANOS_1SEC) {
			if (hero_.hasOverheadMessage() && ++messageCooldown_ == 6) { // si il y a déjà un message sur la tête de notre héros
				messageCooldown_ = 0;
				hero_.setHasOverheadMessage(false);
			}

			Uint64 elapsedFrames = frames_ - lastFrameCount;
			double elapsedSec = static_cast<double>(now - lastSecond) / 1e9;
			fpsResult_ = static_cast<double>(elapsedFrames) / elapsedSec;
			printFPS_Window(); // afficher FPS dans le titre de la fenetre
			if (Config::DEBUG_MODE) std::cout << "FPS = " << fpsResult_ << "\n";
			lastFrameCount = frames_;
			lastSecond += NANOS_1SEC;
		}

		// --- Chaque minute ---
		if (now - lastMinute >= NANOS_1MIN) {
			hero_.addAlignment(1);
			lastMinute += NANOS_1MIN;
		}

		// --- Updates logiques (UPS) ---
		while (now >= nextTick) {
			updateUPS(now);
			nextTick += NANOS_PER_TICK;
			now = nowNanos(); // anti dérive
		}

		// --- Rendu graphique (FPS) ---
		if (now >= nextRender) {
			// calcul du facteur d'interpolation entre deux updates logiques
			double interpolation = static_cast<double>(now - (nextTick - NANOS_PER_TICK)) / static_cast<double>(NANOS_PER_TICK);
			interpolation = std::clamp(interpolation, 0.0, 1.0); // intervalle [0..1]

			camera_.interpolate(interpolation);
			updateFPS();
			nextRender += NANOS_PER_RENDER;

			now = nowNanos(); // important avant le calcul du sleep
		}

		// --- Gestion CPU ---
		Uint64 nextAction = std::min<Uint64>(nextTick, nextRender);
		Sint64 diff = static_cast<Sint64>(nextAction - now);
		if (diff < 0) diff = 0;
		Sint64 sleepNanos = diff - static_cast<Sint64>(overSleepNanos);

		if (sleepNanos > 0) {
			Uint32 sleepMillis = static_cast<Uint32>(sleepNanos / 1'000'000ULL);
			Uint64 before = nowNanos();
			if (sleepMillis > 0) SDL_Delay(sleepMillis);

			// Micro-attente propre sans 100% CPU
			Uint64 remain = static_cast<Uint64>(sleepNanos % 1'000'000ULL);
			if (remain > 0) {
				Uint64 start = nowNanos();
				Uint64 t;
				while ((t = nowNanos()) - start < remain) { std::this_thread::yield(); } // yield CPU
			}

			Uint64 after = nowNanos();
			overSleepNanos = (after - before) - static_cast<Uint64>(sleepNanos); // léger ajustement possible
			noDelays = 0;
		} else {
			overSleepNanos = 0ULL;
			if (++noDelays >= NO_DELAYS_PER_YIELD) {
				SDL_Delay(1); // yield
				std::this_thread::yield();
				noDelays = 0;
			}
		}
	}
}