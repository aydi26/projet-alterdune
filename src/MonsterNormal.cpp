#include "../include/MonsterNormal.h"
#include "../include/Colors.h"

using namespace std;

MonsterNormal::MonsterNormal(string name, int hp, int atk, int def, int mercyGoal, vector<string> acts)
    : Monster(name, hp, atk, def, NORMAL, mercyGoal, acts) {
}

int MonsterNormal::getNbActs() const {
    return 2;
}

string MonsterNormal::getCategoryStr() const {
    return "NORMAL";
}

void MonsterNormal::displayInfo() const {
    cout << BOLD << "=== " << getName() << " ===" << RESET << endl;
    cout << "Categorie : " << WHITE << getCategoryStr() << RESET << endl;
    cout << RED << "HP : " << getHp() << "/" << getHpMax() << RESET << endl;
    cout << CYAN << "ATK : " << getAtk() << " | DEF : " << getDef() << RESET << endl;
    cout << YELLOW << "Mercy : " << getMercy() << "/" << getMercyGoal() << RESET << endl;
}

unique_ptr<Monster> MonsterNormal::clone() const {
    return make_unique<MonsterNormal>(*this);
}
