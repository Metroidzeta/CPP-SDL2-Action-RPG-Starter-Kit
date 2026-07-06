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
#include "headers/Hero.hpp"
#include "headers/Util.hpp"

#include <stdexcept>
#include <sstream>
#include <cstdio> // std::printf

/** Méthodes statiques **/
void Hero::checkArguments(const std::string& name, int level, int goldCoins, int xCell, int yCell, GameMap& gameMap, float critChancePercent) {
	if (name.empty() || Util::isBlank(name)) throw std::invalid_argument("Nom du héros null ou vide");
	if (level < 1 || level > Config::LEVEL_MAX) throw std::invalid_argument("Niveau du héros " + name + " < 1 ou > " + std::to_string(Config::LEVEL_MAX));
	if (goldCoins < 0) throw std::invalid_argument("Pieces d'or du héros " + name + " < 0");
	if (xCell < 0 || xCell >= gameMap.getWidth()) throw std::invalid_argument("Case x du héros " + name + " < 0 ou >= " + std::to_string(gameMap.getWidth()));
	if (yCell < 0 || yCell >= gameMap.getHeight()) throw std::invalid_argument("Case y du héros " + name + " < 0 ou >= " + std::to_string(gameMap.getHeight()));
	if (critChancePercent < 0.f || critChancePercent > 100.f)  throw std::invalid_argument("Pourcentage de chance de coups critiques du héros" + name + " < 0 ou > 100");
}

int Hero::alignToTile(int value) noexcept { return value - (value % Config::CELL_SIZE); }

std::array<SDL_Rect, Hero::DIRECTIONS_COUNT> Hero::createSwordHitBox(int x, int y) noexcept { // HIT BOX EPEE (REELLE / A L'ECRAN)
	const int cellSize = Config::CELL_SIZE;
	return {
		SDL_Rect{ x, y + (cellSize / 2) + OFFSET_SWORD_PX, cellSize, cellSize / 2 }, // BAS
		SDL_Rect{ x - OFFSET_SWORD_PX, y, cellSize / 2, cellSize },                  // GAUCHE
		SDL_Rect{ x + (cellSize / 2) + OFFSET_SWORD_PX, y, cellSize / 2, cellSize }, // DROITE
		SDL_Rect{ x, y - (cellSize / 2) + OFFSET_SWORD_PX, cellSize, cellSize / 2 }  // HAUT
	};
}

/** Constructeur **/
Hero::Hero(const std::string& name, Skin& skin, HeroClass hc, int level, int goldCoins, int xCell, int yCell, GameMap& gameMap, float critChancePercent)
	: name_(name), skin_(&skin), classe_(hc), level_(level), goldCoins_(goldCoins), position_(xCell * Config::CELL_SIZE, yCell * Config::CELL_SIZE),
	screenPosition_(alignToTile(Config::WINDOW_WIDTH / 2), alignToTile(Config::WINDOW_HEIGHT / 2)), currentGameMap_(&gameMap) {
	checkArguments(name, level, goldCoins, xCell, yCell, gameMap, critChancePercent);

	tempHitbox_ = SDL_Rect{ getX(), getY(), Config::CELL_SIZE, Config::CELL_SIZE };
	swordHitbox_ = createSwordHitBox(getX(), getY());
	swordHitboxScreen_ = createSwordHitBox(getXScreen(), getYScreen());
	critChanceRatio_ = critChancePercent / 100.0f; // divise par 100 (pour obtenir un ratio)
	updateOffset();
}

/** Méthodes privées **/
void Hero::incrementMoveFrame() noexcept { moveFrame_ = (moveFrame_ + 1) % 12; }

void Hero::updateOffset() noexcept {
	xOffset_ = screenPosition_.getX() - position_.getX();
	yOffset_ = screenPosition_.getY() - position_.getY();
}

void Hero::updateSwordHitBox() noexcept {
	const int cellSize = Config::CELL_SIZE;
	SDL_Rect& rect = swordHitbox_[static_cast<int>(direction_)];
	const int x = position_.getX(), y = position_.getY();
	switch (direction_) {
		case Directions::DOWN:
			rect.x = x;
			rect.y = y + (cellSize / 2) + OFFSET_SWORD_PX;
			break;
		case Directions::LEFT:
			rect.x = x - OFFSET_SWORD_PX;
			rect.y = y;
			break;
		case Directions::RIGHT:
			rect.x = x + (cellSize / 2) + OFFSET_SWORD_PX;
			rect.y = y;
			break;
		case Directions::UP:
			rect.x = x;
			rect.y = y - (cellSize / 2) + OFFSET_SWORD_PX;
			break;
	}
}

/** Méthodes utilitaires publiques **/
void Hero::updatePosition(int x, int y) noexcept { position_.set(x, y); updateOffset(); }
void Hero::addAlignment(int value)      noexcept { alignGauge_.add(value); }
void Hero::addHp(int value)             noexcept { hpGauge_.add(value); }
void Hero::addMp(int value)             noexcept { mpGauge_.add(value); }
void Hero::levelUp()                    noexcept { if (level_ < Config::LEVEL_MAX) ++level_; }

bool Hero::move(Directions d) {
	tempHitbox_.x = position_.getX(); tempHitbox_.y = position_.getY(); // Copie la position actuelle dans la hitbox temporaire
	const int delta = Config::HERO_MOVE_STEP;
	switch (d) {
		case Directions::DOWN:  tempHitbox_.y += delta; break;
		case Directions::LEFT:  tempHitbox_.x -= delta; break;
		case Directions::RIGHT: tempHitbox_.x += delta; break;
		case Directions::UP:    tempHitbox_.y -= delta; break;
	}

	if (d != direction_) direction_ = d;

	if (tempHitbox_.x >= 0 && tempHitbox_.y >= 0
		&& tempHitbox_.x <= (currentGameMap_->getWidth() - 1) * Config::CELL_SIZE
		&& tempHitbox_.y <= (currentGameMap_->getHeight() - 1) * Config::CELL_SIZE
		&& !currentGameMap_->hasWallCollision(tempHitbox_)) {

		updatePosition(tempHitbox_.x, tempHitbox_.y);
		incrementMoveFrame();
		std::printf("%s s'est deplacé vers %-6s : %s\n", name_.c_str(), ::toString(direction_).data(), position_.toStringCells().c_str());
		return true;
	}

	return false;
}

/** toString() **/
std::string Hero::toString() const {
	std::ostringstream oss;
	oss << "Hero[nom: " << name_ << ", classe: " << classe_ << ", niveau: " << level_ << ", HP: " << hpGauge_ << ", MP: " << mpGauge_ << ", position: " << position_ << ']';
	return oss.str();
}

/** Operator << pour cout **/
std::ostream& operator<<(std::ostream& os, const Hero& hero) {
	return os << hero.toString();
}