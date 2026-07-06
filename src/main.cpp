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
 *
 * To compile on Windows : see README.md
 * To run : see README.md
 */
#include <cstdlib>
#include <exception>
#include <iostream>

#include "headers/SDLContext.hpp"
#include "headers/Game.hpp"

int main(int, char*[]) {
	try {
		SDLContext sdl; // doit être construit en premier
		Game game; // doit être détruit avant SDLContext
		game.play();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}