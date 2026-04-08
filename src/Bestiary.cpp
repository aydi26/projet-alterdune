#include "../include/Bestiary.h"
#include "../include/Colors.h"

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
        cout << YELLOW << "Aucun monstre vaincu pour le moment." << RESET << endl;
        return;
    }

    cout << BOLD << BLUE << "========== BESTIAIRE ==========" << RESET << endl;
    for(int i=0; i < (int)entries.size(); i++) {
        cout << BOLD << "--- " << entries[i].name << " ---" << RESET << endl;
        cout << "Categorie : " << entries[i].category << endl;
        cout << "HP max : " << entries[i].hpMax << " | ATK : " << entries[i].atk << " | DEF : " << entries[i].def << endl;
        if(entries[i].outcome == "Tue") {
            cout << "Resultat : " << RED << entries[i].outcome << RESET << endl;
        } else {
            cout << "Resultat : " << GREEN << entries[i].outcome << RESET << endl;
        }
        cout << endl;
    }
    cout << BOLD << BLUE << "===============================" << RESET << endl;
}

int Bestiary::getSize() const {
    return entries.size();
}
