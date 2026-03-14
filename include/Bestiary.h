#ifndef BESTIARY_H
#define BESTIARY_H

#include "Monster.h"
#include <vector>
#include <string>

// structure pour stocker les infos d'un monstre vaincu
struct BestiaryEntry {
    string name;
    string category;
    int hpMax;
    int atk;
    int def;
    string outcome; // "Tue" ou "Epargne"
};

class Bestiary {
private:
    vector<BestiaryEntry> entries;

public:
    // ajouter un monstre vaincu
    void addEntry(Monster& m, string outcome);

    // afficher le bestiaire
    void display() const;

    // nombre d'entrees
    int getSize() const;
};

#endif
