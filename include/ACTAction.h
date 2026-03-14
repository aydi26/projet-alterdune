#ifndef ACTACTION_H
#define ACTACTION_H

#include <string>

using namespace std;

// structure simple pour stocker une action ACT
struct ACTAction {
    string id;          // identifiant unique (ex: JOKE, COMPLIMENT)
    string text;        // texte affiche quand on fait l'action
    int mercyImpact;    // impact sur la jauge mercy (positif ou negatif)
};

#endif
