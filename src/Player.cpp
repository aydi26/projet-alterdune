#include "../include/Player.h"
#include "../include/Colors.h"

using namespace std;

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
    // on verifie que l'index est valide
    if(index < 0 || index >= (int)inventory.size()) {
        cout << "Index invalide !" << endl;
        return false;
    }

    Item& item = inventory[index];

    if(!item.isUsable()) {
        cout << "Cet item n'est plus disponible !" << endl;
        return false;
    }

    // on applique l'effet selon le type
    if(item.getType() == "HEAL") {
        heal(item.getValue());
        cout << GREEN << "Vous utilisez " << item.getName() << " et recuperez " << item.getValue() << " HP !" << RESET << endl;
        cout << GREEN << "HP : " << hp << "/" << hpMax << RESET << endl;
    } else if(item.getType() == "ATK_BUFF") {
        atkBuff += item.getValue();
        cout << RED << "Vous utilisez " << item.getName() << " ! ATK +" << item.getValue() << " pour ce combat !" << RESET << endl;
    } else if(item.getType() == "DEF_BUFF") {
        defBuff += item.getValue();
        cout << BLUE << "Vous utilisez " << item.getName() << " ! DEF +" << item.getValue() << " pour ce combat !" << RESET << endl;
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
    cout << BOLD << "=== " << name << " ===" << RESET << endl;
    cout << GREEN << "HP : " << hp << "/" << hpMax << RESET << endl;
    cout << CYAN << "ATK : " << atk << " | DEF : " << def << RESET << endl;
    if(atkBuff > 0 || defBuff > 0) {
        cout << CYAN << "Buffs actifs : ATK +" << atkBuff << " | DEF +" << defBuff << RESET << endl;
    }
    cout << RED << "Monstres tues : " << monstersKilled << RESET << endl;
    cout << GREEN << "Monstres epargnes : " << monstersSpared << RESET << endl;
    cout << YELLOW << "Victoires : " << victories << "/10" << RESET << endl;
}

void Player::displayItems() const {
    if(inventory.empty()) {
        cout << "Inventaire vide." << endl;
        return;
    }
    cout << BOLD << "=== Inventaire ===" << RESET << endl;
    for(int i=0; i < (int)inventory.size(); i++) {
        cout << i+1 << ". ";
        inventory[i].displayItem();
    }
}
