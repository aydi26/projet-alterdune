#include "../include/MonsterMiniBoss.h"
#include "../include/Colors.h"
#include "../include/Display.h"
#include <sstream>

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
    Display::drawTopBorder(40, YELLOW);
    Display::drawCenteredLine(BOLD + getName() + RESET, 40, YELLOW);
    Display::drawSeparator(40, YELLOW);
    Display::drawLeftLine("categorie : " + YELLOW + getCategoryStr() + RESET, 40, YELLOW);
    Display::drawLeftLine(Display::hpBar("HP", getHp(), getHpMax(), 16), 40, YELLOW);
    ostringstream statsLine;
    statsLine << CYAN << "ATK : " << getAtk() << "  |  DEF : " << getDef() << RESET;
    Display::drawLeftLine(statsLine.str(), 40, YELLOW);
    Display::drawLeftLine(Display::mercyBar("MERCY", getMercy(), getMercyGoal(), 16), 40, YELLOW);
    Display::drawBottomBorder(40, YELLOW);
}

unique_ptr<Monster> MonsterMiniBoss::clone() const {
    return make_unique<MonsterMiniBoss>(*this);
}
