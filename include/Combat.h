#ifndef COMBAT_H
#define COMBAT_H

#include "Player.h"
#include "Monster.h"
#include "Bestiary.h"
#include "ACTAction.h"
#include <map>
#include <random>

class Combat {
private:
    Player& player;
    Monster monster;    // copie du monstre (on modifie ses HP/mercy pendant le combat)
    map<string, ACTAction>& actCatalog;
    Bestiary& bestiary;
    mt19937& rng;

    // methodes privees pour chaque action
    void displayCombatStatus();
    void doFight();
    void doAct();
    bool doItem();
    bool doMercy();
    void monsterTurn();
    int rollDamage(int hpMax);

public:
    Combat(Player& p, Monster m, map<string, ACTAction>& catalog, Bestiary& best, mt19937& rng);

    // lance le combat, retourne true si le joueur survit
    bool start();
};

#endif
