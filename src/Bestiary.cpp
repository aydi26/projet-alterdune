#include "../include/Bestiary.h"
#include "../include/Colors.h"
#include "../include/Display.h"
#include <sstream>

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

    Display::drawTitleBox("bestiaire", 52, BLUE);
    for(int i=0; i < (int)entries.size(); i++) {
        if(i > 0) {
            Display::drawThinSeparator(52, BLUE);
        }
        Display::drawLeftLine(BOLD + entries[i].name + RESET, 52, BLUE);
        Display::drawLeftLine("categorie : " + entries[i].category, 52, BLUE);
        ostringstream statsLine;
        statsLine << "hp: " << entries[i].hpMax << " | atk: " << entries[i].atk << " | def: " << entries[i].def;
        Display::drawLeftLine(statsLine.str(), 52, BLUE);
        if(entries[i].outcome == "Tue") {
            Display::drawLeftLine("resultat : " + RED + entries[i].outcome + RESET, 52, BLUE);
        } else {
            Display::drawLeftLine("resultat : " + GREEN + entries[i].outcome + RESET, 52, BLUE);
        }
    }
    Display::drawBottomBorder(52, BLUE);
}

int Bestiary::getSize() const {
    return entries.size();
}
