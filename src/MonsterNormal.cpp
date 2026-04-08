#include "../include/MonsterNormal.h"
#include "../include/Colors.h"

using namespace std;

MonsterNormal::MonsterNormal(string name, int hp, int atk,int def,int mercyGoal, vector<string> acts)
    : Monster(name, hp, atk, def, NORMAL, mercyGoal, acts) {
    // toute l'initialisation est déléguée au constructeur parent
}

int MonsterNormal::getNbActs() const {
    return 2;       // 2 actions ACT pour un monstre standard
}

string MonsterNormal::getCategoryStr() const {
    return "NORMAL";
}

void MonsterNormal::displayInfo() const {
    cout << BOLD << "=== " << getName() << " ===" << RESET << endl;
    cout << "categorie : " << WHITE << getCategoryStr() << RESET << endl;
    cout << RED << "hp : " << getHp() << "/" << getHpMax() << RESET << endl;
    cout << CYAN << "atk : " << getAtk() << " | def : " << getDef() << RESET << endl;
    cout << YELLOW << "mercy : " << getMercy() << "/" << getMercyGoal() << RESET << endl;
}

unique_ptr<Monster> MonsterNormal::clone() const {
    return make_unique<MonsterNormal>(*this);
}
