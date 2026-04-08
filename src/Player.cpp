#include "../include/Player.h"
#include "../include/Colors.h"

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
    cout << BOLD << "=== " << name << " ===" << RESET << endl;
    cout << GREEN << "hp : " << hp << "/" << hpMax << RESET << endl;
    cout << CYAN << "atk : " << atk << " | def : " << def << RESET << endl;
    if(atkBuff > 0 || defBuff > 0) {
        cout << CYAN << "buffs actifs : atk +" << atkBuff << " | def +" << defBuff << RESET << endl;
    }
    cout << RED << "monstres tues : " << monstersKilled << RESET << endl;
    cout << GREEN << "monstres epargnes : " << monstersSpared << RESET << endl;
    cout << YELLOW << "victoires : " << victories << "/10" << RESET << endl;
}

void Player::displayItems() const {
    if(inventory.empty()) {
        cout << "inventaire vide." << endl;
        return;
    }
    cout << BOLD << "=== inventaire ===" << RESET << endl;
    for(int i=0; i < (int)inventory.size(); i++) {
        cout << i+1 << ". ";
        inventory[i].displayItem();
    }
}
