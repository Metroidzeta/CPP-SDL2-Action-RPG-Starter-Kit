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
#include <format> // std::format

#include "headers/Camera.hpp"

/** Définit la cible suivie */
void Camera::setTarget(CamLock* target) {
	target_ = target;
	if (target_) {
		xCurr_ = target_->getXCam();
		yCurr_ = target_->getYCam();
		sync(); // synchronisation totale
	}
}

/** Mise à jour logique */
void Camera::update() {
	if (!target_) return;
	xPrev_ = xCurr_;
	yPrev_ = yCurr_;
	xCurr_ = target_->getXCam();
	yCurr_ = target_->getYCam();
}

/** Interpolation */
void Camera::interpolate(double interpolation) noexcept {
	xInter_ = xPrev_ + (xCurr_ - xPrev_) * interpolation;
	yInter_ = yPrev_ + (yCurr_ - yPrev_) * interpolation;
}

/** Synchronisation complète */
void Camera::sync() noexcept {
	xPrev_ = xCurr_;
	yPrev_ = yCurr_;
	xInter_ = xCurr_;
	yInter_ = yCurr_;
}

/** toString() */
std::string Camera::toString() const {
	return std::format(
		"Camera[Prev: (x: {:.2f}, y: {:.2f}), Inter: (x: {:.2f}, y: {:.2f}), Curr: (x: {}, y: {})]",
		xPrev_, yPrev_, xInter_, yInter_, xCurr_, yCurr_
	);
}

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
	return os << camera.toString();
}