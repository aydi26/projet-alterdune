#include "../include/MonsterNormal.h"
#include "../include/Colors.h"
#include "../include/Display.h"
#include <sstream>

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
    Display::drawTopBorder(40, WHITE);
    Display::drawCenteredLine(BOLD + getName() + RESET, 40, WHITE);
    Display::drawSeparator(40, WHITE);
    Display::drawLeftLine("categorie : " + WHITE + getCategoryStr() + RESET, 40, WHITE);
    Display::drawLeftLine(Display::hpBar("HP", getHp(), getHpMax(), 16), 40, WHITE);
    ostringstream statsLine;
    statsLine << CYAN << "ATK : " << getAtk() << "  |  DEF : " << getDef() << RESET;
    Display::drawLeftLine(statsLine.str(), 40, WHITE);
    Display::drawLeftLine(Display::mercyBar("MERCY", getMercy(), getMercyGoal(), 16), 40, WHITE);
    Display::drawBottomBorder(40, WHITE);
}

unique_ptr<Monster> MonsterNormal::clone() const {
    return make_unique<MonsterNormal>(*this);
}
