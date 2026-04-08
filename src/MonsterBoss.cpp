#include "../include/MonsterBoss.h"
#include "../include/Colors.h"

using namespace std;

MonsterBoss::MonsterBoss(string name, int hp, int atk, int def, int mercyGoal, vector<string> acts)
    : Monster(name, hp, atk, def, BOSS, mercyGoal, acts) {
}

int MonsterBoss::getNbActs() const {
    return 4;
}

string MonsterBoss::getCategoryStr() const {
    return "BOSS";
}

void MonsterBoss::displayInfo() const {
    cout << BOLD << "=== " << getName() << " ===" << RESET << endl;
    cout << "Categorie : " << RED << getCategoryStr() << RESET << endl;
    cout << RED << "HP : " << getHp() << "/" << getHpMax() << RESET << endl;
    cout << CYAN << "ATK : " << getAtk() << " | DEF : " << getDef() << RESET << endl;
    cout << YELLOW << "Mercy : " << getMercy() << "/" << getMercyGoal() << RESET << endl;
}

unique_ptr<Monster> MonsterBoss::clone() const {
    return make_unique<MonsterBoss>(*this);
}
