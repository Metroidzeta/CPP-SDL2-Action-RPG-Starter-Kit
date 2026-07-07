![C++/SDL2-Action-RPG-Starter-Kit-screen-1](https://raw.githubusercontent.com/Metroidzeta/CPP-SDL2-Action-RPG-Starter-Kit/refs/heads/main/screenshots/C%2B%2BSDL2-Action-RPG-Starter-Kit-screen-1.png)
![C++/SDL2-Action-RPG-Starter-Kit-screen-2](https://raw.githubusercontent.com/Metroidzeta/CPP-SDL2-Action-RPG-Starter-Kit/refs/heads/main/screenshots/C%2B%2BSDL2-Action-RPG-Starter-Kit-screen-2.png)

---
🇫🇷 Version Française

# C++/SDL2 Action RPG Starter Kit v1.0
Projet de création d'un moteur de jeu 2D Action-RPG inspiré du MMORPG français *Slayers Online*  
(version offline pour le moment, il n'est pas en ligne).

Le moteur est entièrement conçu et codé en C++/SDL2 from scratch (de 0) par Metroidzeta (Alain Barbier).  

## POUR COMPILER LE PROGRAMME

Tout d’abord compiler avec Windows (à partir du dossier racine) :
```g++ -std=c++20 -Wall -Wextra -Wpedantic src/*.cpp src/loaders/*.cpp src/resources/*.cpp src/renderer/*.cpp -o prog.exe -Isrc -Isrc/loaders -Isrc/resources -Isrc/renderer -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image```  
Puis exécuter :
```./prog```  

## 🎮 CONTRÔLES

**Flèches directionnelles ← ↑ ↓ → :** bouger (diagonale possible)  
**Touche A :** voir les murs  
**Touche B :** mettre le jeu en plein écran / remettre en fenêtré  
**Touche S :** attaquer (avec une hitbox visible)  
**Touche ENTREE :** écrire un message (puis le poster)  
**Touche ECHAP :** voir le menu (par défaut les statistiques)  
**Touche F3 :** réécrire le message précédent  
**Touche F5 :** changer la couleur des cadres de l’interface  
**Touche Q :** quitter le jeu (/!\ attention, cette touche quitte le jeu instantanément)  

## TÉLÉCHARGER LE JEU

[Télécharger le jeu en entier ici (v1.0)](https://mega.nz/file/M40njBSC#y13s695fb631OQKmV_lzmkk2s3Q75_KUKYXaaBqVYLU)

## POUR LANCER LE JEU

Depuis la racine du projet, exécuter le fichier ```prog.exe```

## 📜 LICENCE

Ce projet est distribué sous la Licence d’Utilisation Personnelle et Éducative
(*Educational and Personal Use License*).

Vous pouvez lire, étudier, forker et modifier le code à titre personnel ou éducatif.

Vous ne pouvez pas publier, redistribuer ni utiliser ce code dans un cadre commercial ou public, sauf autorisation écrite de l’auteur.

Les ressources graphiques, musicales et sonores appartiennent à leurs auteurs respectifs et ne sont pas couvertes par cette licence.

👉 Voir le fichier complet [LICENSE.md](https://github.com/Metroidzeta/CPP-SDL2-Action-RPG-Starter-Kit/blob/main/LICENSE.md) pour les termes détaillés (versions française et anglaise).

📧 Contact : [Alain1293@outlook.fr](mailto:Alain1293@outlook.fr)

© 2026 Alain Barbier alias Metroidzeta – Tous droits réservés.

---
🇬🇧 English Version

# C++/SDL2 Action RPG Starter Kit v1.0

Project for creating a 2D Action-RPG game engine inspired by the French MMORPG *Slayers Online*  
(offline version for now, it is not online yet).

The engine is entirely designed and coded in C++/SDL2 from scratch (from 0) by Metroidzeta (Alain Barbier).

## HOW TO COMPILE THE PROGRAM

First, compile on Windows (from the project root folder) :
```g++ -std=c++20 -Wall -Wextra -Wpedantic src/*.cpp src/loaders/*.cpp src/resources/*.cpp src/renderer/*.cpp -o prog.exe -Isrc -Isrc/loaders -Isrc/resources -Isrc/renderer -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image```

Then run :
```./prog```

## 🎮 CONTROLS

**Arrow keys ← ↑ ↓ →:** move (including diagonal movement)  
**A key:** show walls  
**B key:** toggle fullscreen/windowed mode  
**S key:** attack (with a visible hitbox)  
**ENTER key:** write a message (then send it)  
**ESC key:** open the menu (showing statistics by default)  
**F3 key:** rewrite the previous message  
**F5 key:** change the UI box color  
**Q key:** quit the game (/!\ warning: this key closes the game instantly)  

## DOWNLOAD THE GAME

[Download the full game here (v1.0)](https://mega.nz/file/M40njBSC#y13s695fb631OQKmV_lzmkk2s3Q75_KUKYXaaBqVYLU)

## HOW TO LAUNCH THE GAME

From the project root folder, run the `prog.exe` file.

## 📜 LICENSE

This project is distributed under the Educational and Personal Use License  
(*Licence d’Utilisation Personnelle et Éducative*).

You may read, study, fork, and modify the code for personal or educational purposes.

You may not publish, redistribute, or use this code in a commercial or public context without the author’s written permission.

The graphics, music, and sound assets belong to their respective authors and are not covered by this license.

👉 See the full [LICENSE.md](https://github.com/Metroidzeta/CPP-SDL2-Action-RPG-Starter-Kit/blob/main/LICENSE.md) file for detailed terms in both French and English.

📧 Contact: [Alain1293@outlook.fr](mailto:Alain1293@outlook.fr)

© 2026 Alain Barbier alias Metroidzeta – All rights reserved.