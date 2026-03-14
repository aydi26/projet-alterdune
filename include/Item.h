#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

using namespace std;

class Item {
private:
    string name;
    string type;
    int value;
    int quantity;

public:
    // constructeur
    Item(string name, string type, int value, int quantity);

    // getters
    string getName() const;
    string getType() const;
    int getValue() const;
    int getQuantity() const;

    // utiliser l'item (decremente la quantite)
    void use();

    // verifier si on peut encore l'utiliser
    bool isUsable() const;

    // afficher les infos de l'item
    void displayItem() const;
};

#endif
