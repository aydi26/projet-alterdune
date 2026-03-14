#include "../include/Player.h"

using namespace std;

Player::Player(string name) : Entity(name, 100, 10, 5) {
    victories = 0;
    monstersKilled = 0;
    monstersSpared = 0;
}

Player::Player() : Entity() {
    victories = 0;
    monstersKilled = 0;
    monstersSpared = 0;
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
        cout << "Vous utilisez " << item.getName() << " et recuperez " << item.getValue() << " HP !" << endl;
        cout << "HP : " << hp << "/" << hpMax << endl;
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

void Player::displayInfo() const {
    cout << "=== " << name << " ===" << endl;
    cout << "HP : " << hp << "/" << hpMax << endl;
    cout << "ATK : " << atk << " | DEF : " << def << endl;
    cout << "Monstres tues : " << monstersKilled << endl;
    cout << "Monstres epargnes : " << monstersSpared << endl;
    cout << "Victoires : " << victories << "/10" << endl;
}

void Player::displayItems() const {
    if(inventory.empty()) {
        cout << "Inventaire vide." << endl;
        return;
    }
    cout << "=== Inventaire ===" << endl;
    for(int i=0; i < (int)inventory.size(); i++) {
        cout << i+1 << ". ";
        inventory[i].displayItem();
    }
}
