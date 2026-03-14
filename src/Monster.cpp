#include "../include/Monster.h"

using namespace std;

Monster::Monster(string name, int hp, int atk, int def, MonsterCategory cat, int mercyGoal, vector<string> acts)
    : Entity(name, hp, atk, def) {
    this->category = cat;
    this->mercy = 0;
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
    // on borne entre 0 et mercyGoal
    if(mercy < 0) mercy = 0;
    if(mercy > mercyGoal) mercy = mercyGoal;
}

bool Monster::canBeSpared() const {
    return mercy >= mercyGoal;
}

int Monster::getNbActs() const {
    // polymorphisme : le nombre d'actions depend de la categorie
    switch(category) {
        case NORMAL: return 2;
        case MINIBOSS: return 3;
        case BOSS: return 4;
        default: return 2;
    }
}

void Monster::resetForCombat() {
    hp = hpMax;
    mercy = 0;
}

MonsterCategory Monster::getCategory() const {
    return category;
}

string Monster::getCategoryStr() const {
    switch(category) {
        case NORMAL: return "NORMAL";
        case MINIBOSS: return "MINIBOSS";
        case BOSS: return "BOSS";
        default: return "NORMAL";
    }
}

int Monster::getMercy() const { return mercy; }
int Monster::getMercyGoal() const { return mercyGoal; }
vector<string> Monster::getActIDs() const { return actIDs; }

void Monster::displayInfo() const {
    cout << "=== " << name << " ===" << endl;
    cout << "Categorie : " << getCategoryStr() << endl;
    cout << "HP : " << hp << "/" << hpMax << endl;
    cout << "ATK : " << atk << " | DEF : " << def << endl;
    cout << "Mercy : " << mercy << "/" << mercyGoal << endl;
}
