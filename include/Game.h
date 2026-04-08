#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Monster.h"
#include "MonsterNormal.h"
#include "MonsterMiniBoss.h"
#include "MonsterBoss.h"
#include "Bestiary.h"
#include "Combat.h"
#include "ACTAction.h"
#include <vector>
#include <map>
#include <random>
#include <string>
#include <memory>

// structure pour un evenement aleatoire
struct RandomEvent {
    string name;
    string description;
    string type;    // "HEAL", "DAMAGE", "ATK_BOOST", "DEF_BOOST", "ITEM_FIND"
    int value;
};

class Game {
private:
    Player player;
    vector<unique_ptr<Monster>> monsterPool;
    Bestiary bestiary;
    map<string, ACTAction> actCatalog;
    vector<RandomEvent> eventCatalog;
    mt19937 rng;
    bool gameOver;

    // initialiser les catalogues
    void initActCatalog();
    void initEventCatalog();

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

    // evenements aleatoires entre combats
    void triggerRandomEvent();

    // convertir un string en categorie
    MonsterCategory parseCategory(string cat);

public:
    Game();
    void run();
};

#endif
