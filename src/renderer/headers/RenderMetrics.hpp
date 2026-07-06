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

#include "../../headers/Config.hpp"

namespace RenderMetrics {
	namespace Detail {
		constexpr int wh(double ratio) noexcept { return static_cast<int>(Config::WINDOW_HEIGHT * ratio); }
		constexpr int ww(double ratio) noexcept { return static_cast<int>(Config::WINDOW_WIDTH  * ratio); }
	}

	/** Pourcentages de la hauteur de la fenêtre **/
	inline constexpr int WH_1PERCENT  = Detail::wh(0.01);
	inline constexpr int WH_2PERCENT  = Detail::wh(0.02);
	inline constexpr int WH_4PERCENT  = Detail::wh(0.04);
	inline constexpr int WH_5PERCENT  = Detail::wh(0.05);
	inline constexpr int WH_8PERCENT  = Detail::wh(0.08);
	inline constexpr int WH_20PERCENT = Detail::wh(0.20);
	inline constexpr int WH_26PERCENT = Detail::wh(0.26);
	inline constexpr int WH_37PERCENT = Detail::wh(0.37);
	inline constexpr int WH_69PERCENT = Detail::wh(0.69);
	inline constexpr int WH_94PERCENT = Detail::wh(0.94);
	inline constexpr int WH_95PERCENT = Detail::wh(0.95);
	inline constexpr int WH_98PERCENT = Detail::wh(0.98);

	/** Pourcentages de la largeur de la fenêtre **/
	inline constexpr int WW_1PERCENT    = Detail::ww(0.01);
	inline constexpr int WW_2PERCENT    = Detail::ww(0.02);
	inline constexpr int WW_3PERCENT    = Detail::ww(0.03);
	inline constexpr int WW_4PERCENT    = Detail::ww(0.04);
	inline constexpr int WW_15PERCENT   = Detail::ww(0.15);
	inline constexpr int WW_17PERCENT   = Detail::ww(0.17);
	inline constexpr int WW_17_5PERCENT = Detail::ww(0.175);
	inline constexpr int WW_65PERCENT   = Detail::ww(0.65);
	inline constexpr int WW_80PERCENT   = Detail::ww(0.80);
	inline constexpr int WW_84PERCENT   = Detail::ww(0.84);
	inline constexpr int WW_94PERCENT   = Detail::ww(0.94);
	inline constexpr int WW_96PERCENT   = Detail::ww(0.96);
}