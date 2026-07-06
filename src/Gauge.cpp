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
#include <functional> // std::hash
#include <sstream> // std::ostringstream
#include <cstdint> // std::int64_t

#include "headers/Gauge.hpp"

/** Constructeur **/
Gauge::Gauge(int value, int max)
	: value_(value), max_(max) {
	if (max_ < 1) throw std::invalid_argument("Le max doit être >= 1");
	if (value_ < 0 || value_ > max_) throw std::invalid_argument("La valeur doit être comprise entre 0 et " + std::to_string(max));
}

/** Méthodes utilitaires **/
void Gauge::add(int delta) noexcept {
	std::int64_t result = static_cast<std::int64_t>(value_) + delta;
	result = std::clamp(result, std::int64_t{0}, static_cast<std::int64_t>(max_));
	value_ = static_cast<int>(result);
}

/** Operator==, hash(), toString() **/
bool Gauge::operator==(const Gauge& other) const noexcept {
	return value_ == other.value_
		&& max_ == other.max_;
}

std::size_t Gauge::hash() const noexcept {
	return std::hash<int>()(value_) ^ (std::hash<int>()(max_) << 1);
}

std::string Gauge::toString() const {
	std::ostringstream oss;
	oss << '(' << value_ << '/' << max_ << ')';
	return oss.str();
}

/** Operator<< (pour cout) **/
std::ostream& operator<<(std::ostream& os, const Gauge& j) {
	return os << j.toString();
}