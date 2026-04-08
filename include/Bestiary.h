#ifndef BESTIARY_H
#define BESTIARY_H

#include "Monster.h"
#include <vector>
#include <string>

// entrée du bestiaire pour les monstres déjà rencontrés
// on stocke les valeurs par copie pour s'affranchir des problèmes de durée de vie
struct BestiaryEntry {
    string name;
    string category;
    int hpMax;
    int atk;
    int def;
    string outcome;     // "Tue" ou "Epargne"
};

class Bestiary {
private:
    vector<BestiaryEntry> entries;

public:
    // ajoute une entrée à la fin de chaque combat, appelé depuis Combat
    void addEntry(Monster& m, string outcome);

    void display() const;

    int getSize() const;
};

#endif
