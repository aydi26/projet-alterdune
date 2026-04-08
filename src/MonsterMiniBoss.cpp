#include "../include/MonsterMiniBoss.h"
#include "../include/Colors.h"

using namespace std;

MonsterMiniBoss::MonsterMiniBoss(string name, int hp, int atk, int def, int mercyGoal, vector<string> acts)
    : Monster(name, hp, atk, def, MINIBOSS, mercyGoal, acts) {
}

int MonsterMiniBoss::getNbActs() const {
    return 3;
}

string MonsterMiniBoss::getCategoryStr() const {
    return "MINIBOSS";
}

void MonsterMiniBoss::displayInfo() const {
    cout << BOLD << "=== " << getName() << " ===" << RESET << endl;
    cout << "Categorie : " << YELLOW << getCategoryStr() << RESET << endl;
    cout << RED << "HP : " << getHp() << "/" << getHpMax() << RESET << endl;
    cout << CYAN << "ATK : " << getAtk() << " | DEF : " << getDef() << RESET << endl;
    cout << YELLOW << "Mercy : " << getMercy() << "/" << getMercyGoal() << RESET << endl;
}

unique_ptr<Monster> MonsterMiniBoss::clone() const {
    return make_unique<MonsterMiniBoss>(*this);
}
