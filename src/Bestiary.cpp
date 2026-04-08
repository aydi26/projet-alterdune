#include "../include/Bestiary.h"
#include "../include/Colors.h"

using namespace std;

// on copie les informations du monstre dans une struct dédiée
// le passage par valeur évite tout pointeur pendant si le monstre est détruit par la suite
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
        cout << YELLOW << "aucun monstre vaincu pour le moment." << RESET << endl;
        return;
    }

    cout << BOLD << BLUE << "========== bestiaire ==========" << RESET << endl;
    for(int i=0; i < (int)entries.size(); i++) {
        cout << BOLD << "--- " << entries[i].name << " ---" << RESET << endl;
        cout << "categorie : " << entries[i].category << endl;
        cout << "hp max : " << entries[i].hpMax << " | atk : " << entries[i].atk << " | def : " << entries[i].def << endl;
        // résultat coloré selon l'issue : rouge pour un monstre tué, vert pour un monstre épargné
        if(entries[i].outcome == "Tue") {
            cout << "resultat : " << RED << entries[i].outcome << RESET << endl;
        } else {
            cout << "resultat : " << GREEN << entries[i].outcome << RESET << endl;
        }
        cout << endl;
    }
    cout << BOLD << BLUE << "===============================" << RESET << endl;
}

int Bestiary::getSize() const {
    return entries.size();
}
