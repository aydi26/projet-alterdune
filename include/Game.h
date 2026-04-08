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

// description d'un événement aléatoire pouvant survenir entre deux combats
// struct simple suffisante pour ce besoin, pas de logique associée
struct RandomEvent {
    string name;
    string description;
    string type;        // "HEAL", "DAMAGE", "ATK_BOOST", "DEF_BOOST", "ITEM_FIND"
    int value;
};

class Game {
private:
    Player player;
    vector<unique_ptr<Monster>> monsterPool;    // pool de monstres chargés depuis le csv
    Bestiary bestiary;
    map<string, ACTAction> actCatalog;          // catalogue des actions ACT disponibles dans le jeu
    vector<RandomEvent> eventCatalog;
    mt19937 rng;
    bool gameOver;

    // initialisation des catalogues en dur (pas de fichier de données dédié pour ces deux-là)
    void initActCatalog();
    void initEventCatalog();

    // chargement des fichiers de données externes
    bool loadItems(string filename);
    bool loadMonsters(string filename);

    // écrans accessibles depuis le menu principal
    void showMenu();
    void startCombat();
    void showStats();
    void showItems();
    void showBestiary();

    // affiche l'écran de fin selon le profil du joueur (pacifiste / génocidaire / neutre)
    void checkEnding();

    // déclenche un événement aléatoire après un combat avec une probabilité fixée
    void triggerRandomEvent();

    // utilitaire : convertit la chaîne lue dans le csv en enum MonsterCategory
    MonsterCategory parseCategory(string cat);

public:
    Game();
    void run();
};

#endif
