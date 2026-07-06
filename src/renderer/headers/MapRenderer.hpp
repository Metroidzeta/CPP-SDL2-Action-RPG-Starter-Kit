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

#include <array>

class Camera;
class Hero;
class RendererContext;

/**
 * Gestion du rendu de la carte.
 */
class MapRenderer final {
private:
	static constexpr int X0 = 0, X1 = 1, Y0 = 2, Y1 = 3;

	Camera& camera_;
	Hero& hero_;
	RendererContext& ctx_;

	std::array<int, 4> bounds_{};

public:
	/** Constructeur **/
	MapRenderer(Camera& camera, Hero& hero, RendererContext& ctx) noexcept;

	/** Calcule les bornes d'affichage visibles par la caméra. **/
	void calculateDisplayBounds();

	/** Affichage - Carte **/
	void drawLayer(int layerIndex) const;
	void drawWalls() const;
};