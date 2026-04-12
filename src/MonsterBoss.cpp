#include "../include/MonsterBoss.h"
#include "../include/Colors.h"
#include "../include/Display.h"
#include <sstream>

using namespace std;

MonsterBoss::MonsterBoss(string name, int hp,int atk, int def, int mercyGoal, vector<string> acts)
    : Monster(name, hp, atk, def, BOSS, mercyGoal, acts) {
}

int MonsterBoss::getNbActs() const {
    return 4;       // un boss expose la totalité de son panel d'actions
}

string MonsterBoss::getCategoryStr() const {
    return "BOSS";
}

void MonsterBoss::displayInfo() const {
    Display::drawTopBorder(40, RED);
    Display::drawCenteredLine(BOLD + getName() + RESET, 40, RED);
    Display::drawSeparator(40, RED);
    Display::drawLeftLine("categorie : " + RED + getCategoryStr() + RESET, 40, RED);
    Display::drawLeftLine(Display::hpBar("HP", getHp(), getHpMax(), 16), 40, RED);
    ostringstream statsLine;
    statsLine << CYAN << "ATK : " << getAtk() << "  |  DEF : " << getDef() << RESET;
    Display::drawLeftLine(statsLine.str(), 40, RED);
    Display::drawLeftLine(Display::mercyBar("MERCY", getMercy(), getMercyGoal(), 16), 40, RED);
    Display::drawBottomBorder(40, RED);
}

unique_ptr<Monster> MonsterBoss::clone() const {
    return make_unique<MonsterBoss>(*this);
}
