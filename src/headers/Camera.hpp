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
#include <ostream>

/**
* Interface de la cible suivie par la caméra.
*/
class CamLock {
public:
	virtual ~CamLock() = default;
	virtual int getXCam() const noexcept = 0;
	virtual int getYCam() const noexcept = 0;
};

/**
* Caméra 2D fluide capable de suivre dynamiquement une entité "CamLock".
* Elle interpole la position de la cible pour obtenir un mouvement fluide,
* même à des fréquences de rendu supérieures au taux de mise à jour logique.
*/
class Camera final {
private:
	CamLock* target_ = nullptr;  // pointeur non-owning sur la cible (pas de delete)

	double xPrev_ = 0.0, yPrev_ = 0.0;
	double xInter_ = 0.0, yInter_ = 0.0;
	int xCurr_ = 0, yCurr_ = 0;

public:
	Camera() = default;

	/** Getters */
	double getX() const noexcept { return xInter_; }
	double getY() const noexcept { return yInter_; }

	/** Définit la cible à suivre */
	void setTarget(CamLock* target);

	/** Mise à jour logique (appelée à chaque update logique) */
	void update();

	/** Interpolation entre positions précédente et actuelle */
	void interpolate(double interpolation) noexcept;

	/** Synchronisation complète (utile après TP / changement de carte) */
	void sync() noexcept;

	/** toString() */
	std::string toString() const;
	friend std::ostream& operator<<(std::ostream& os, const Camera& camera);
};