#ifndef ACTACTION_H
#define ACTACTION_H

#include <string>

using namespace std;

// structure POD décrivant une action ACT
// id pour la lookup dans la map du catalogue, texte affiché en jeu, et impact sur la jauge mercy
struct ACTAction {
    string id;
    string text;
    int mercyImpact;    // peut être négatif pour les actions hostiles (ex: INSULT, THREATEN)
};

#endif
