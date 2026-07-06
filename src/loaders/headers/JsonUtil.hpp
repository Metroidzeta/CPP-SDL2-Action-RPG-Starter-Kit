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

#include <nlohmann/json.hpp>

namespace JsonUtil {
	inline bool isIntKey(const nlohmann::json& obj, const char* key) {
		return obj.contains(key) && obj.at(key).is_number_integer();
	}

	inline bool isStringKey(const nlohmann::json& obj, const char* key) {
		return obj.contains(key) && obj.at(key).is_string();
	}

	inline bool isArrayKey(const nlohmann::json& obj, const char* key) {
		return obj.contains(key) && obj.at(key).is_array();
	}
}