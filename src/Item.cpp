#include "../include/Item.h"
#include "../include/Colors.h"

using namespace std;

Item::Item(string name, string type, int value, int quantity) {
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
    if(type == "HEAL") {
        cout << GREEN << "Soigne " << value << " HP" << RESET;
    } else if(type == "ATK_BUFF") {
        cout << RED << "Augmente ATK de " << value << RESET;
    } else if(type == "DEF_BUFF") {
        cout << BLUE << "Augmente DEF de " << value << RESET;
    }
    cout << " - Quantite: " << quantity << endl;
}
