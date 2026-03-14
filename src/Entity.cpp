#include "../include/Entity.h"

using namespace std;

Entity::Entity(string name, int hp, int atk, int def) {
    this->name = name;
    this->hp = hp;
    this->hpMax = hp;
    this->atk = atk;
    this->def = def;
}

Entity::Entity() {
    name = "";
    hp = 0;
    hpMax = 0;
    atk = 0;
    def = 0;
}

void Entity::takeDamage(int dmg) {
    hp -= dmg;
    if(hp < 0) hp = 0;
}

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::heal(int amount) {
    hp += amount;
    if(hp > hpMax) hp = hpMax;
}

// getters
string Entity::getName() const { return name; }
int Entity::getHp() const { return hp; }
int Entity::getHpMax() const { return hpMax; }
int Entity::getAtk() const { return atk; }
int Entity::getDef() const { return def; }

// setters
void Entity::setName(string n) { name = n; }
void Entity::setHp(int h) { hp = h; }
