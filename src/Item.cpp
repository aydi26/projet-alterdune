#include "../include/Item.h"
#include "../include/Colors.h"

using namespace std;

Item::Item(string name, string type,int value, int quantity) {
    this->name = name;
    this->type = type;
    this->value = value;
    this->quantity = quantity;
}

string Item::getName() const {
    return name;
}

string Item::getType() const {
    return type;
}

int Item::getValue() const {
    return value;
}

int Item::getQuantity() const {
    return quantity;
}

// décrémente la quantité, avec une garde pour éviter de descendre sous 0
void Item::use() {
    if(quantity > 0) {
        quantity--;
    }
}

bool Item::isUsable() const {
    return quantity > 0;
}

void Item::displayItem() const {
    cout << name << " (" << type << ") - ";
    // coloration en fonction du type pour améliorer la lisibilité de l'inventaire
    if(type == "HEAL") {
        cout << GREEN << "soigne " << value << " hp" << RESET;
    } else if(type == "ATK_BUFF") {
        cout << RED << "augmente atk de " << value << RESET;
    } else if(type == "DEF_BUFF") {
        cout << BLUE << "augmente def de " << value << RESET;
    }
    cout << " - quantite: " << quantity << endl;
}
