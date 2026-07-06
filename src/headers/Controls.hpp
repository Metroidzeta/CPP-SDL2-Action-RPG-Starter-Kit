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

#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

class Hero; // Déclaration anticipée

/**
 * Gère l’ensemble des contrôles clavier du jeu.
 *
 * Cette classe centralise l’état des touches pressées via un modèle de type Singleton.
 * Elle traduit les codes clavier SDL2 (SDL_Event) en touches logiques du jeu,
 * et permet d’associer un héros ainsi qu’un buffer de message pour la saisie de texte en jeu.
 *
 * Fournit également des méthodes utilitaires simples (UP(), DOWN(), A(), etc.)
 * pour interroger directement l’état des commandes dans la boucle principale.
 */
class Controls final {

public:
	/** Touches reconnues **/
	enum class GameKey {
		UP, DOWN, LEFT, RIGHT,
		A, B, Q, S,
		SPACE, ENTER, ESCAPE, BACK_SPACE,
		F1, F3, F5
	};

	/** Copie et assignation interdite **/
	Controls(const Controls&) = delete;
	Controls& operator=(const Controls&) = delete;

	/** Getters **/
	static Controls& getInstance(); // Récupération de l'instance unique (singleton)
	bool isPressed(GameKey key) const noexcept; // Vérifie si une touche est enfoncée

	/** Setters **/
	void setTargets(Hero* hero, std::u32string* message) noexcept;

	/** Méthodes utilitaires **/
	void handleEvent(const SDL_Event& e); // Détection des touches
	void consume(GameKey key) noexcept; // Consomme une touche en la retirant de l’ensemble des touches pressées

	/** Getters directs **/
	bool UP()             const noexcept { return isPressed(GameKey::UP); }
	bool DOWN()           const noexcept { return isPressed(GameKey::DOWN); }
	bool LEFT()           const noexcept { return isPressed(GameKey::LEFT); }
	bool RIGHT()          const noexcept { return isPressed(GameKey::RIGHT); }
	bool A()              const noexcept { return isPressed(GameKey::A); }
	bool B()              const noexcept { return isPressed(GameKey::B); }
	bool Q()              const noexcept { return isPressed(GameKey::Q); }
	bool S()              const noexcept { return isPressed(GameKey::S); }
	bool SPACE()          const noexcept { return isPressed(GameKey::SPACE); }
	bool ENTER()          const noexcept { return isPressed(GameKey::ENTER); }
	bool ESCAPE()         const noexcept { return isPressed(GameKey::ESCAPE); }
	bool BACK_SPACE()     const noexcept { return isPressed(GameKey::BACK_SPACE); }
	bool F1()             const noexcept { return isPressed(GameKey::F1); }
	bool F3()             const noexcept { return isPressed(GameKey::F3); }
	bool F5()             const noexcept { return isPressed(GameKey::F5); }

private:
	/** Constructeur privé **/
	Controls() = default;

	static char32_t decodeUTF8(const char* bytes) noexcept; // Décode un seul caractère UTF-8 en codepoint (1-4 octets)

private:
	std::unordered_set<GameKey> states_; // Ensemble des touches pressées
	static const std::unordered_map<SDL_Keycode, GameKey> keyMap; // Map (SDL_Keycode -> Touche)

	Hero* hero_ = nullptr;
	std::u32string* message_ = nullptr; // Buffer texte UTF-32
};
