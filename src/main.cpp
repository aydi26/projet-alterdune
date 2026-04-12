#include "../include/Game.h"
#include "../include/Display.h"

// point d'entrée du programme : toute la logique est déléguée à Game::run pour garder main minimal
int main() {
    Display::initConsole();
    Game game;
    game.run();
    return 0;
}
