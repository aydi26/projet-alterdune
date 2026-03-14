#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Item.h"
#include <vector>

class Player : public Entity {
private:
    vector<Item> inventory;
    int victories;
    int monstersKilled;
    int monstersSpared;

public:
    // constructeur avec nom
    Player(string name);
    // constructeur par defaut
    Player();

    // gestion de l'inventaire
    void addItem(Item item);
    vector<Item>& getInventory();

    // utiliser un item par son index dans l'inventaire
    bool useItem(int index);

    // compteurs
    void addVictory();
    void addKill();
    void addSpare();

    // getters
    int getVictories() const;
    int getMonstersKilled() const;
    int getMonstersSpared() const;

    // affichage
    void displayInfo() const override;
    void displayItems() const;
};

#endif
