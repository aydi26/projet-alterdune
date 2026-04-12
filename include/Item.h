#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

using namespace std;

// représente un item de l'inventaire joueur
// types supportés : "HEAL", "ATK_BUFF", "DEF_BUFF"
class Item {
private:
    string name;
    string type;
    int value;     // valeur de l'effet (montant de soin ou de buff)
    int quantity;  // si == 0, l'item est consommé et ne peut plus être utilisé

public:
    Item(string name, string type, int value,int quantity);

    // getters
    string getName() const;
    string getType() const;
    int getValue() const;
    int getQuantity() const;

    // décrémente la quantité d'une unité (consomme l'item)
    void use();

    // true tant qu'il reste au moins une unité
    bool isUsable() const;

    // affichage formaté avec couleur en fonction du type
    void displayItem() const;

    // retourne la chaine formatee (sans endl) pour integration dans un cadre
    string formatItem() const;
};

#endif
