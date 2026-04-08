#ifndef COMBAT_H
#define COMBAT_H

#include "Player.h"
#include "Monster.h"
#include "Bestiary.h"
#include "ACTAction.h"
#include <map>
#include <random>
#include <memory>

// classe responsable du déroulement d'un combat unique
// instanciée à chaque nouveau combat depuis Game::startCombat
class Combat {
private:
    Player& player;                         // référence pour modifier directement l'instance joueur
    unique_ptr<Monster> monster;            // prise de propriété du monstre cloné le temps du combat
    map<string, ACTAction>& actCatalog;     // catalogue partagé, considéré comme immuable ici
    Bestiary& bestiary;
    mt19937& rng;                           // générateur partagé avec Game pour préserver l'état de l'aléa

    // sous-routines internes, une par option du menu de combat
    void displayCombatStatus();
    void doFight();
    void doAct();
    bool doItem();
    bool doMercy();
    void monsterTurn();

    // calcul des dégâts selon la formule du sujet : aléa(0..hpMax) + atk - def, minimum 1 si non raté
    int rollDamage(int hpMax, int attackerAtk, int defenderDef);

public:
    Combat(Player& p, unique_ptr<Monster> m,map<string, ACTAction>& catalog, Bestiary& best,mt19937& rng);

    // lance la boucle principale du combat ; retourne true si le joueur survit
    bool start();
};

#endif
