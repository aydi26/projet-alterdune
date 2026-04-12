#include "../include/Player.h"
#include "../include/Colors.h"
#include "../include/Display.h"
#include <sstream>

using namespace std;

// statistiques de départ : 100 hp / 10 atk / 5 def
// valeurs calibrées pour rester équilibrées face au pool de monstres défini dans le csv
Player::Player(string name) : Entity(name, 100, 10, 5) {
    victories = 0;
    monstersKilled = 0;
    monstersSpared = 0;
    atkBuff = 0;
    defBuff = 0;
}

Player::Player() : Entity() {
    victories = 0;
    monstersKilled = 0;
    monstersSpared = 0;
    atkBuff = 0;
    defBuff = 0;
}

void Player::addItem(Item item) {
    inventory.push_back(item);
}

vector<Item>& Player::getInventory() {
    return inventory;
}

bool Player::useItem(int index) {
    // validation de l'index pour éviter tout accès hors limites
    if(index < 0 || index >= (int)inventory.size()) {
        cout << "index invalide !" << endl;
        return false;
    }

    Item& item = inventory[index];

    // l'item peut exister dans l'inventaire mais avoir une quantité nulle s'il a déjà été consommé
    if(!item.isUsable()) {
        cout << "cet item n'est plus disponible !" << endl;
        return false;
    }

    // application de l'effet selon le type
    // chaîne de if/else if préférée à un dispatch polymorphe pour rester simple sur 3 types seulement
    if(item.getType() == "HEAL") {
        heal(item.getValue());
        cout << GREEN << "tu utilises " << item.getName() << " et tu recuperes " << item.getValue() << " hp !" << RESET << endl;
        cout << GREEN << "hp : " << hp << "/" << hpMax << RESET << endl;
    } else if(item.getType() == "ATK_BUFF") {
        atkBuff += item.getValue();
        cout << RED << "tu utilises " << item.getName() << " ! atk +" << item.getValue() << " pour ce combat" << RESET << endl;
    } else if(item.getType() == "DEF_BUFF") {
        defBuff += item.getValue();
        cout << BLUE << "tu utilises " << item.getName() << " ! def +" << item.getValue() << " pour ce combat" << RESET << endl;
    }

    item.use();
    return true;
}

void Player::addVictory() { victories++; }
void Player::addKill() { monstersKilled++; }
void Player::addSpare() { monstersSpared++; }

int Player::getVictories() const { return victories; }
int Player::getMonstersKilled() const { return monstersKilled; }
int Player::getMonstersSpared() const { return monstersSpared; }

int Player::getAtkBuff() const { return atkBuff; }
int Player::getDefBuff() const { return defBuff; }
void Player::setAtkBuff(int val) { atkBuff = val; }
void Player::setDefBuff(int val) { defBuff = val; }
void Player::resetBuffs() { atkBuff = 0; defBuff = 0; }

void Player::displayInfo() const {
    Display::drawTopBorder(52, CYAN);
    Display::drawCenteredLine(BOLD + name + RESET, 52, CYAN);
    Display::drawSeparator(52, CYAN);
    Display::drawLeftLine(Display::hpBar("HP", hp, hpMax), 52, CYAN);
    ostringstream statsLine;
    statsLine << CYAN << "ATK : " << atk << "  |  DEF : " << def << RESET;
    Display::drawLeftLine(statsLine.str(), 52, CYAN);
    if(atkBuff > 0 || defBuff > 0) {
        ostringstream buffLine;
        buffLine << YELLOW << "buffs actifs : atk +" << atkBuff << " | def +" << defBuff << RESET;
        Display::drawLeftLine(buffLine.str(), 52, CYAN);
    }
    Display::drawSeparator(52, CYAN);
    ostringstream killLine;
    killLine << RED << "monstres tues : " << monstersKilled << RESET;
    Display::drawLeftLine(killLine.str(), 52, CYAN);
    ostringstream spareLine;
    spareLine << GREEN << "monstres epargnes : " << monstersSpared << RESET;
    Display::drawLeftLine(spareLine.str(), 52, CYAN);
    ostringstream victLine;
    victLine << YELLOW << "victoires : " << victories << "/10" << RESET;
    Display::drawLeftLine(victLine.str(), 52, CYAN);
    Display::drawBottomBorder(52, CYAN);
}

void Player::displayItems() const {
    if(inventory.empty()) {
        cout << YELLOW << "inventaire vide." << RESET << endl;
        return;
    }
    Display::drawTitleBox("inventaire", 52, GREEN);
    for(int i=0; i < (int)inventory.size(); i++) {
        ostringstream oss;
        oss << i+1 << ". " << inventory[i].formatItem();
        Display::drawLeftLine(oss.str(), 52, GREEN);
    }
    Display::drawBottomBorder(52, GREEN);
}
