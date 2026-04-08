#include "../include/Monster.h"
#include <memory>

using namespace std;

Monster::Monster(string name, int hp,int atk,int def, MonsterCategory cat,int mercyGoal, vector<string> acts)
    : Entity(name, hp, atk, def) {
    this->category = cat;
    this->mercy = 0;        // initialisée à 0 en début de combat
    this->mercyGoal = mercyGoal;
    this->actIDs = acts;
}

Monster::Monster() : Entity() {
    category = NORMAL;
    mercy = 0;
    mercyGoal = 100;
}

void Monster::addMercy(int amount) {
    mercy += amount;
    // clamp dans l'intervalle [0, mercyGoal] pour éviter tout dépassement de borne
    if(mercy < 0) mercy = 0;
    if(mercy > mercyGoal) mercy = mercyGoal;
}

bool Monster::canBeSpared() const {
    return mercy >= mercyGoal;
}

// implémentation par défaut, basée sur la catégorie
// les sous-classes redéfinissent cette méthode mais ce fallback garantit un comportement cohérent
int Monster::getNbActs() const {
    switch(category) {
        case NORMAL:   return 2;
        case MINIBOSS: return 3;
        case BOSS:     return 4;
        default:       return 2;
    }
}

// réinitialise l'état du monstre pour permettre sa réutilisation à travers plusieurs combats
void Monster::resetForCombat() {
    hp = hpMax;
    mercy = 0;
}

MonsterCategory Monster::getCategory() const {
    return category;
}

string Monster::getCategoryStr() const {
    switch(category) {
        case NORMAL:   return "NORMAL";
        case MINIBOSS: return "MINIBOSS";
        case BOSS:     return "BOSS";
        default:       return "NORMAL";
    }
}

int Monster::getMercy() const { return mercy; }
int Monster::getMercyGoal() const { return mercyGoal; }
vector<string> Monster::getActIDs() const { return actIDs; }

unique_ptr<Monster> Monster::clone() const {
    return make_unique<Monster>(*this);
}

void Monster::displayInfo() const {
    cout << "=== " << name << " ===" << endl;
    cout << "categorie : " << getCategoryStr() << endl;
    cout << "hp : " << hp << "/" << hpMax << endl;
    cout << "atk : " << atk << " | def : " << def << endl;
    cout << "mercy : " << mercy << "/" << mercyGoal << endl;
}
