#ifndef COMBAT_H
#define COMBAT_H

#include "Player.h"
#include "Monster.h"
#include "Bestiary.h"
#include "ACTAction.h"
#include <map>
#include <random>
#include <memory>

class Combat {
private:
    Player& player;
    unique_ptr<Monster> monster;    // monstre polymorphe (on modifie ses HP/mercy pendant le combat)
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
    int rollDamage(int hpMax, int attackerAtk, int defenderDef);

public:
    Combat(Player& p, unique_ptr<Monster> m, map<string, ACTAction>& catalog, Bestiary& best, mt19937& rng);

    // lance le combat, retourne true si le joueur survit
    bool start();
};

#endif
