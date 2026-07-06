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
#include <stdexcept>
#include <utility> // std::move, std::exchange
#include <format> // std::format

#include "../headers/Config.hpp"
#include "../headers/Util.hpp"
#include "headers/Font.hpp"

/** Constructeur **/
Font::Font(const std::string& fileName, int size)
	: name_(fileName), size_(size) {
	if (name_.empty() || Util::isBlank(name_)) throw std::invalid_argument("Nom du fichier de la font vide");
	if (size_ <= 0) throw std::invalid_argument("Taille de font <= 0");

	const std::string path = std::string(Config::FONTS_DIRECTORY) + '/' + fileName;
	font_ = TTF_OpenFont(path.c_str(), size_);
	if (!font_) throw std::runtime_error(std::format("Echec chargement font : {} | TTF_GetError: {}", fileName, TTF_GetError()));
}

/** Destructeur **/
Font::~Font() noexcept { TTF_CloseFont(font_); }

/** Move constructor **/
Font::Font(Font&& other) noexcept
	: name_(std::move(other.name_)),
	  size_(std::exchange(other.size_, 0)),
	  font_(std::exchange(other.font_, nullptr)) {}

/** Move assignment **/
Font& Font::operator=(Font&& other) noexcept {
	if (this != &other) {
		TTF_CloseFont(font_);
		name_ = std::move(other.name_);
		size_ = std::exchange(other.size_, 0);
		font_ = std::exchange(other.font_, nullptr);
	}
	return *this;
}