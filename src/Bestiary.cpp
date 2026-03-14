#include "../include/Bestiary.h"

using namespace std;

void Bestiary::addEntry(Monster& m, string outcome) {
    BestiaryEntry entry;
    entry.name = m.getName();
    entry.category = m.getCategoryStr();
    entry.hpMax = m.getHpMax();
    entry.atk = m.getAtk();
    entry.def = m.getDef();
    entry.outcome = outcome;
    entries.push_back(entry);
}

void Bestiary::display() const {
    if(entries.empty()) {
        cout << "Aucun monstre vaincu pour le moment." << endl;
        return;
    }

    cout << "========== BESTIAIRE ==========" << endl;
    for(int i=0; i < (int)entries.size(); i++) {
        cout << "--- " << entries[i].name << " ---" << endl;
        cout << "Categorie : " << entries[i].category << endl;
        cout << "HP max : " << entries[i].hpMax << " | ATK : " << entries[i].atk << " | DEF : " << entries[i].def << endl;
        cout << "Resultat : " << entries[i].outcome << endl;
        cout << endl;
    }
    cout << "===============================" << endl;
}

int Bestiary::getSize() const {
    return entries.size();
}
