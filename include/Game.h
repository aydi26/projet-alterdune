#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Monster.h"
#include "Bestiary.h"
#include "Combat.h"
#include "ACTAction.h"
#include <vector>
#include <map>
#include <random>
#include <string>

class Game {
private:
    Player player;
    vector<Monster> monsterPool;
    Bestiary bestiary;
    map<string, ACTAction> actCatalog;
    mt19937 rng;
    bool gameOver;

    // initialiser le catalogue d'actions ACT
    void initActCatalog();

    // charger les fichiers CSV
    bool loadItems(string filename);
    bool loadMonsters(string filename);

    // fonctions du menu
    void showMenu();
    void startCombat();
    void showStats();
    void showItems();
    void showBestiary();

    // verifier et afficher la fin
    void checkEnding();

    // convertir un string en categorie
    MonsterCategory parseCategory(string cat);

public:
    Game();
    void run();
};

#endif
