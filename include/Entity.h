#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>

using namespace std;

// classe de base abstraite pour toutes les entités combattantes (joueur, monstres)
// regroupe les attributs et comportements communs pour éviter la duplication
class Entity {
protected:
    string name;
    int hp;
    int hpMax;
    int atk;
    int def;

public:
    // constructeur principal
    Entity(string name,int hp, int atk,int def);

    // constructeur par défaut, nécessaire pour pouvoir déclarer un Player non initialisé dans Game
    Entity();

    // destructeur virtuel obligatoire pour une bonne destruction polymorphe
    virtual ~Entity() {}

    // actions de base
    void takeDamage(int dmg);
    bool isAlive() const;
    void heal(int amount);

    // getters
    string getName() const;
    int getHp() const;
    int getHpMax() const;
    int getAtk() const;
    int getDef() const;

    // setters
    void setName(string n);
    void setHp(int h);

    // méthode virtuelle pure : rend la classe abstraite et force l'implémentation dans les filles
    virtual void displayInfo() const = 0;
};

#endif
