#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Item.h"
#include <vector>

// classe joueur, hérite d'Entity
// possède un inventaire et des compteurs servant à déterminer la fin du jeu
class Player : public Entity {
private:
    vector<Item> inventory;
    int victories;
    int monstersKilled;
    int monstersSpared;

    // buffs temporaires, réinitialisés après chaque combat
    // permet d'éviter une accumulation déséquilibrée via les events aléatoires
    int atkBuff;
    int defBuff;

public:
    Player(string name);
    Player();   // constructeur par défaut, requis pour le membre Game::player

    // gestion de l'inventaire
    void addItem(Item item);
    vector<Item>& getInventory();

    // utilise l'item situé à l'index donné, retourne false si aucune action n'a été effectuée
    bool useItem(int index);

    // compteurs utilisés pour le scoring de fin de partie
    void addVictory();
    void addKill();
    void addSpare();

    int getVictories() const;
    int getMonstersKilled() const;
    int getMonstersSpared() const;

    // gestion des buffs temporaires
    int getAtkBuff() const;
    int getDefBuff() const;
    void setAtkBuff(int val);
    void setDefBuff(int val);
    void resetBuffs();

    // affichage
    void displayInfo() const override;
    void displayItems() const;
};

#endif
