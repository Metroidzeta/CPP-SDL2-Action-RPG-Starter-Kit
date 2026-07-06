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
#include "headers/Config.hpp"
#include "headers/Controls.hpp"
#include "headers/Hero.hpp"

/** Récupération de l'instance unique (singleton) **/
Controls& Controls::getInstance() {
	static Controls instance;
	return instance;
}

void Controls::setTargets(Hero* hero, std::u32string* message) noexcept {
	hero_ = hero;
	message_ = message;
}

/** Mapping KeyCode -> GameKey **/
const std::unordered_map<SDL_Keycode, Controls::GameKey> Controls::keyMap = {
	{SDLK_UP,        GameKey::UP},
	{SDLK_DOWN,      GameKey::DOWN},
	{SDLK_LEFT,      GameKey::LEFT},
	{SDLK_RIGHT,     GameKey::RIGHT},
	{SDLK_a,         GameKey::A},
	{SDLK_b,         GameKey::B},
	{SDLK_q,         GameKey::Q},
	{SDLK_s,         GameKey::S},
	{SDLK_SPACE,     GameKey::SPACE},
	{SDLK_RETURN,    GameKey::ENTER},
	{SDLK_ESCAPE,    GameKey::ESCAPE},
	{SDLK_BACKSPACE, GameKey::BACK_SPACE},
	{SDLK_F1,        GameKey::F1},
	{SDLK_F3,        GameKey::F3},
	{SDLK_F5,        GameKey::F5}
};

bool Controls::isPressed(GameKey key) const noexcept { return states_.contains(key); } // Vérifie si une touche est enfoncée
void Controls::consume(GameKey key) noexcept { states_.erase(key); } // Consomme une touche en la retirant de l’ensemble des touches pressées

char32_t Controls::decodeUTF8(const char* bytes) noexcept {
	const unsigned char c = bytes[0];

	if (c < 0x80) return c; // 1 octet

	if ((c & 0xE0) == 0xC0) // 2 octets
		return ((c & 0x1F) << 6) | (bytes[1] & 0x3F);

	if ((c & 0xF0) == 0xE0) // 3 octets
		return ((c & 0x0F) << 12)
			 | ((bytes[1] & 0x3F) << 6)
			 |  (bytes[2] & 0x3F);

	if ((c & 0xF8) == 0xF0) // 4 octets
		return ((c & 0x07) << 18)
			 | ((bytes[1] & 0x3F) << 12)
			 | ((bytes[2] & 0x3F) << 6)
			 |  (bytes[3] & 0x3F);

	return U'?';
}

void Controls::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) { // Si une touche est pressée
		if (auto it = keyMap.find(e.key.keysym.sym); it != keyMap.end()) states_.insert(it->second);
	}

	else if (e.type == SDL_KEYUP) { // Si une touche est relachée
		if (auto it = keyMap.find(e.key.keysym.sym); it != keyMap.end()) states_.erase(it->second);
	}

	else if (e.type == SDL_TEXTINPUT) { // Si un caractère est tapé (écriture)
		if (!hero_ || !message_ || !hero_->isWriting()) return;

		char32_t ch = decodeUTF8(e.text.text);
		if (ch < 32 || ch == 127) return; // caractère autorisée UTF-8

		if (message_->size() < Config::MSG_SIZE_MAX) message_->push_back(ch);
	}
}