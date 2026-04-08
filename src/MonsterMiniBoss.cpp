#include "../include/MonsterMiniBoss.h"
#include "../include/Colors.h"

using namespace std;

MonsterMiniBoss::MonsterMiniBoss(string name,int hp, int atk, int def, int mercyGoal,vector<string> acts)
    : Monster(name, hp, atk, def, MINIBOSS, mercyGoal, acts) {
}

int MonsterMiniBoss::getNbActs() const {
    return 3;       // un mini-boss propose un panel d'actions un peu plus large qu'un monstre normal
}

string MonsterMiniBoss::getCategoryStr() const {
    return "MINIBOSS";
}

void MonsterMiniBoss::displayInfo() const {
    cout << BOLD << "=== " << getName() << " ===" << RESET << endl;
    cout << "categorie : " << YELLOW << getCategoryStr() << RESET << endl;
    cout << RED << "hp : " << getHp() << "/" << getHpMax() << RESET << endl;
    cout << CYAN << "atk : " << getAtk() << " | def : " << getDef() << RESET << endl;
    cout << YELLOW << "mercy : " << getMercy() << "/" << getMercyGoal() << RESET << endl;
}

unique_ptr<Monster> MonsterMiniBoss::clone() const {
    return make_unique<MonsterMiniBoss>(*this);
}
