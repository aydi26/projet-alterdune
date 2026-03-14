#include "../include/Combat.h"
#include <limits>

using namespace std;

Combat::Combat(Player& p, Monster m, map<string, ACTAction>& catalog, Bestiary& best, mt19937& rng)
    : player(p), monster(m), actCatalog(catalog), bestiary(best), rng(rng) {
}

int Combat::rollDamage(int hpMax) {
    uniform_int_distribution<int> dist(0, hpMax);
    return dist(rng);
}

void Combat::displayCombatStatus() {
    cout << endl;
    cout << "--- Etat du combat ---" << endl;
    cout << player.getName() << " : HP " << player.getHp() << "/" << player.getHpMax() << endl;
    cout << monster.getName() << " : HP " << monster.getHp() << "/" << monster.getHpMax();
    cout << " | Mercy : " << monster.getMercy() << "/" << monster.getMercyGoal() << endl;
    cout << "----------------------" << endl;
}

void Combat::doFight() {
    int degats = rollDamage(monster.getHpMax());

    if(degats == 0) {
        cout << "Vous attaquez " << monster.getName() << " mais vous ratez votre coup !" << endl;
    } else {
        cout << "Vous attaquez " << monster.getName() << " et infligez " << degats << " degats !" << endl;
        monster.takeDamage(degats);
        if(!monster.isAlive()) {
            cout << monster.getName() << " est vaincu !" << endl;
        }
    }
}

void Combat::doAct() {
    vector<string> acts = monster.getActIDs();
    int nbActs = monster.getNbActs();

    // afficher les actions disponibles
    cout << "Actions disponibles :" << endl;
    for(int i=0; i < nbActs && i < (int)acts.size(); i++) {
        cout << i+1 << ". " << acts[i] << endl;
    }

    // choix du joueur
    int choix;
    cout << "Choisissez une action : ";
    while(!(cin >> choix) || choix < 1 || choix > nbActs) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Choix invalide, reessayez : ";
    }

    string actID = acts[choix-1];

    // chercher l'action dans le catalogue
    if(actCatalog.find(actID) != actCatalog.end()) {
        ACTAction& action = actCatalog[actID];
        cout << endl << action.text << endl;
        monster.addMercy(action.mercyImpact);

        if(action.mercyImpact > 0) {
            cout << "(Mercy +" << action.mercyImpact << ")" << endl;
        } else if(action.mercyImpact < 0) {
            cout << "(Mercy " << action.mercyImpact << ")" << endl;
        }
    } else {
        cout << "Action inconnue..." << endl;
    }
}

bool Combat::doItem() {
    vector<Item>& inv = player.getInventory();

    // verifier si l'inventaire a des items utilisables
    bool hasUsable = false;
    for(int i=0; i < (int)inv.size(); i++) {
        if(inv[i].isUsable()) hasUsable = true;
    }

    if(!hasUsable) {
        cout << "Vous n'avez aucun item utilisable !" << endl;
        return false;
    }

    // afficher les items
    player.displayItems();

    int choix;
    cout << "Choisissez un item (0 pour annuler) : ";
    while(!(cin >> choix) || choix < 0 || choix > (int)inv.size()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Choix invalide, reessayez : ";
    }

    if(choix == 0) return false;

    return player.useItem(choix-1);
}

bool Combat::doMercy() {
    if(monster.canBeSpared()) {
        cout << "Vous epargnez " << monster.getName() << " !" << endl;
        cout << monster.getName() << " s'en va paisiblement..." << endl;
        return true;
    } else {
        cout << "La jauge Mercy n'est pas assez haute ! (" << monster.getMercy() << "/" << monster.getMercyGoal() << ")" << endl;
        return false;
    }
}

void Combat::monsterTurn() {
    int degats = rollDamage(player.getHpMax());

    if(degats == 0) {
        cout << monster.getName() << " vous attaque mais rate son coup !" << endl;
    } else {
        cout << monster.getName() << " vous attaque et inflige " << degats << " degats !" << endl;
        player.takeDamage(degats);
    }
}

bool Combat::start() {
    cout << endl;
    cout << "==============================" << endl;
    cout << "Un " << monster.getName() << " (" << monster.getCategoryStr() << ") apparait !" << endl;
    cout << "==============================" << endl;

    bool spared = false;

    while(monster.isAlive() && player.isAlive() && !spared) {
        displayCombatStatus();

        // menu de combat
        cout << endl;
        cout << "1. FIGHT" << endl;
        cout << "2. ACT" << endl;
        cout << "3. ITEM" << endl;
        cout << "4. MERCY" << endl;
        cout << "Que voulez-vous faire ? ";

        int choix;
        while(!(cin >> choix) || choix < 1 || choix > 4) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Choix invalide, reessayez : ";
        }

        bool turnUsed = true; // est-ce que le tour a ete consomme

        switch(choix) {
            case 1:
                doFight();
                break;
            case 2:
                doAct();
                break;
            case 3:
                if(!doItem()) {
                    turnUsed = false; // l'item n'a pas ete utilise, on recommence
                }
                break;
            case 4:
                if(doMercy()) {
                    spared = true;
                } else {
                    turnUsed = false; // mercy echouee, on recommence
                }
                break;
        }

        // si le monstre est mort, on arrete
        if(!monster.isAlive()) {
            player.addVictory();
            player.addKill();
            bestiary.addEntry(monster, "Tue");
            cout << endl << "Vous avez tue " << monster.getName() << " !" << endl;
            cout << "Victoires : " << player.getVictories() << "/10" << endl;
            return true;
        }

        // si le monstre est epargne
        if(spared) {
            player.addVictory();
            player.addSpare();
            bestiary.addEntry(monster, "Epargne");
            cout << endl << "Vous avez epargne " << monster.getName() << " !" << endl;
            cout << "Victoires : " << player.getVictories() << "/10" << endl;
            return true;
        }

        // tour du monstre (seulement si le tour a ete consomme)
        if(turnUsed) {
            cout << endl;
            monsterTurn();

            // verifier si le joueur est mort
            if(!player.isAlive()) {
                cout << endl << "Vous avez ete vaincu par " << monster.getName() << "..." << endl;
                cout << "GAME OVER" << endl;
                return false;
            }
        }
    }

    return true;
}
