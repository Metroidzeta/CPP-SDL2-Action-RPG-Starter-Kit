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
#include <array>
#include <ostream>
#include <SDL2/SDL.h>

#include "Directions.hpp"
#include "HeroClass.hpp"
#include "Gauge.hpp"
#include "Position.hpp"
#include "Camera.hpp"
#include "Skin.hpp"
#include "GameMap.hpp"
#include "Config.hpp"

/**
 * Représente le héros contrôlé par le joueur : position, statistiques et état de jeu.
 */
class Hero final : public CamLock {
private:
	struct Statistics {
		static constexpr int FORCE        = 12;
		static constexpr int DEXTERITY    = 9;
		static constexpr int CONSTITUTION = 10;
		static constexpr int ALIGNMENT    = 50;
		static constexpr int HP           = 600;
		static constexpr int MP           = 250;
	};

	static constexpr int OFFSET_SWORD_PX  = Config::CELL_SIZE / 4;
	static constexpr int DIRECTIONS_COUNT = 4; // par défaut : 4

	/** Méthodes privées statics **/
	static void checkArguments(const std::string& name, int level, int goldCoins, int xCell, int yCell, GameMap& gameMap, float critChancePercent);
	static int alignToTile(int value) noexcept;
	static std::array<SDL_Rect, DIRECTIONS_COUNT> createSwordHitBox(int x, int y) noexcept;

	std::string name_;
	Skin* skin_ = nullptr;
	HeroClass classe_;
	int level_ = 1;
	int goldCoins_ = 0;
	Position position_; // position réelle
	Position screenPosition_; // position SUR L'ECRAN
	int xOffset_ = 0, yOffset_ = 0; // décalage entre position réelle (x,y) et la position (x,y) SUR L'ECRAN
	int force_ = Statistics::FORCE;
	int dexterity_ = Statistics::DEXTERITY;
	int constitution_ = Statistics::CONSTITUTION;
	Gauge hpGauge_ { Statistics::HP, Statistics::HP }; // HP / HPMax
	Gauge mpGauge_ { Statistics::MP, Statistics::MP }; // MP / MPMax
	Directions direction_ = Directions::DOWN; // regarde vers le bas par défaut
	Gauge alignGauge_ { Statistics::ALIGNMENT, 100 };
	bool canSwordAttack_ = false, swordAttack_ = false, isBlocked_ = false, hasOverheadMessage_ = false, isWriting_ = false, isInEvent_ = false;
	GameMap* currentGameMap_ = nullptr;
	float critChanceRatio_ = 0.0f; // ratio [0.0 - 100.0]

	SDL_Rect tempHitbox_{};
	std::array<SDL_Rect, DIRECTIONS_COUNT> swordHitbox_{};
	std::array<SDL_Rect, DIRECTIONS_COUNT> swordHitboxScreen_{};
	int moveFrame_ = 7;

	/** Méthodes privées **/
	void incrementMoveFrame() noexcept;
	void updateOffset() noexcept;

public:
	/** Constructeurs **/
	Hero() = delete; // vide interdit
	Hero(const std::string& name, Skin& skin, HeroClass hc, int level, int goldCoins, int xCell, int yCell, GameMap& gameMap, float critChancePercent);

	/** Copie interdite **/
	Hero(const Hero&) = delete;
	Hero& operator=(const Hero&) = delete;

	/** Getters **/
	const std::string& getName()                const noexcept { return name_; }
	Skin&              getSkin()                const noexcept { return *skin_; }
	HeroClass          getHeroClass()           const noexcept { return classe_; }
	int                getLevel()               const noexcept { return level_; }
	int                getGoldCoins()           const noexcept { return goldCoins_; }
	const Position&    getPosition()            const noexcept { return position_; }
	int                getXOffset()             const noexcept { return xOffset_; }
	int                getYOffset()             const noexcept { return yOffset_; }
	int                getX()                   const noexcept { return position_.getX(); }
	int                getY()                   const noexcept { return position_.getY(); }
	int                getXCell()               const noexcept { return position_.getX() / Config::CELL_SIZE; }
	int                getYCell()               const noexcept { return position_.getY() / Config::CELL_SIZE; }
	std::string        getPositionCellsString() const          { return position_.toStringCells(); }
	const Position&    getScreenPosition()      const noexcept { return screenPosition_; }
	int                getXScreen()             const noexcept { return screenPosition_.getX(); }
	int                getYScreen()             const noexcept { return screenPosition_.getY(); }
	int                getXCam()       const noexcept override { return xOffset_; }
	int                getYCam()       const noexcept override { return yOffset_; }
	int                getForce()               const noexcept { return force_; }
	int                getDexterity()           const noexcept { return dexterity_; }
	int                getConstitution()        const noexcept { return constitution_; }
	int                getHp()                  const noexcept { return hpGauge_.getValue(); }
	int                getHpMax()               const noexcept { return hpGauge_.getMax(); }
	double             getHpRatio()             const noexcept { return hpGauge_.getRatio(); }
	int                getMp()                  const noexcept { return mpGauge_.getValue(); }
	int                getMpMax()               const noexcept { return mpGauge_.getMax(); }
	double             getMpRatio()             const noexcept { return mpGauge_.getRatio(); }
	Directions         getDirection()           const noexcept { return direction_; }
	int                getAlignment()           const noexcept { return alignGauge_.getValue(); }
	bool               canSwordAttack()         const noexcept { return canSwordAttack_; }
	bool               swordAttack()            const noexcept { return swordAttack_; }
	bool               isBlocked()              const noexcept { return isBlocked_; }
	bool               hasOverheadMessage()     const noexcept { return hasOverheadMessage_; }
	bool               isWriting()              const noexcept { return isWriting_; }
	bool               isInEvent()              const noexcept { return isInEvent_; }
	GameMap&           getCurrentGameMap()      const noexcept { return *currentGameMap_; }
	float              getCritChanceRatio()     const noexcept { return critChanceRatio_; }
	SDL_Rect           getHitbox()              const noexcept { return position_.getRectHitbox(Config::CELL_SIZE); }
	SDL_Rect           getSwordHitbox()         const noexcept { return swordHitbox_[static_cast<int>(direction_)]; }
	SDL_Rect           getSwordHitboxScreen()   const noexcept { return swordHitboxScreen_[static_cast<int>(direction_)]; }
	int                getMoveFrame()           const noexcept { return moveFrame_; }

	/** Setters **/
	void setCanSwordAttack(bool b)           noexcept { canSwordAttack_     = b; }
	void setSwordAttack(bool b)              noexcept { swordAttack_        = b; }
	void setIsBlocked(bool b)                noexcept { isBlocked_          = b; }
	void setHasOverheadMessage(bool b)       noexcept { hasOverheadMessage_ = b; }
	void setIsWriting(bool b)                noexcept { isWriting_          = b; }
	void setIsInEvent(bool b)                noexcept { isInEvent_          = b; }
	void setCurrentGameMap(GameMap& gameMap) noexcept { currentGameMap_     = &gameMap; }
	void setMoveFrame(int mf)                noexcept { moveFrame_          = mf; }

	/** Méthodes utilitaires **/
	void updatePosition(int x, int y) noexcept;
	void addAlignment(int value) noexcept;
	void addHp(int value) noexcept;
	void addMp(int value) noexcept;
	void levelUp() noexcept;
	void updateSwordHitBox() noexcept;
	bool move(Directions d); // retourne true si un déplacement ou un changement de direction a eu lieu

	//** toString() **/
	std::string toString() const;
};

/** Opérateur << **/
std::ostream& operator<<(std::ostream& os, const Hero& hero);