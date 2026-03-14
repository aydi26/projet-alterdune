#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>

using namespace std;

// classe abstraite : base pour Player et Monster
class Entity {
protected:
    string name;
    int hp;
    int hpMax;
    int atk;
    int def;

public:
    // constructeur
    Entity(string name, int hp, int atk, int def);
    // constructeur par defaut (pour pouvoir declarer sans initialiser)
    Entity();

    // destructeur virtuel
    virtual ~Entity() {}

    // methodes communes
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

    // methode virtuelle pure -> rend la classe abstraite
    virtual void displayInfo() const = 0;
};

#endif
