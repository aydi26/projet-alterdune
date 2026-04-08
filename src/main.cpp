#include "../include/Game.h"

// point d'entrée du programme : toute la logique est déléguée à Game::run pour garder main minimal
int main() {
    Game game;
    game.run();
    return 0;
}
