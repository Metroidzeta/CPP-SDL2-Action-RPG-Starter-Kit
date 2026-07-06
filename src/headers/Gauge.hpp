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

#include <stdexcept>
#include <string>
#include <algorithm> // std::clamp
#include <cstddef> // std::size_t
#include <ostream> // std::ostream

/**
 * Représente une jauge bornée entre 0 et un maximum
 * (par exemple : HP, MP, etc.).
 */
class Gauge final {
private:
	int value_, max_;

public:
	/** Constructeur **/
	Gauge() = delete; // constructeur vide interdit
	Gauge(int value, int max);

	/** Getters **/
	int    getValue() const noexcept { return value_; }
	int    getMax()   const noexcept { return max_; }
	double getRatio() const noexcept { return static_cast<double>(value_) / max_; }

	/** Setters **/
	void setValue(int newValue) noexcept { value_ = std::clamp(newValue, 0, max_); }
	void setMax(int newMax)     noexcept { max_ = std::max(1, newMax); value_ = std::min(value_, max_); }

	/** Méthodes utilitaires **/
	bool isFull()  const noexcept { return value_ == max_; }
	bool isEmpty() const noexcept { return value_ == 0; }
	void add(int delta)  noexcept;

	/** Operator ==, !=, hash(), toString() **/
	bool operator==(const Gauge& other) const noexcept;
	bool operator!=(const Gauge& other) const noexcept { return !(*this == other); }
	std::size_t hash() const noexcept;
	std::string toString() const;
};

/** Operator << (pour cout) **/
std::ostream& operator<<(std::ostream& os, const Gauge& j);