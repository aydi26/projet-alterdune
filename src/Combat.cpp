#include "../include/Combat.h"
#include "../include/Colors.h"
#include <limits>

using namespace std;

Combat::Combat(Player& p, unique_ptr<Monster> m, map<string, ACTAction>& catalog, Bestiary& best, mt19937& rng)
    : player(p), monster(move(m)), actCatalog(catalog), bestiary(best), rng(rng) {
}

int Combat::rollDamage(int hpMax, int attackerAtk, int defenderDef) {
    uniform_int_distribution<int> dist(0, hpMax);
    int baseDamage = dist(rng);
    if(baseDamage == 0) return 0; // coup rate
    int finalDamage = baseDamage + attackerAtk - defenderDef;
    if(finalDamage < 1) finalDamage = 1; // minimum 1 degat si pas rate
    return finalDamage;
}

void Combat::displayCombatStatus() {
    cout << endl;
    cout << BOLD << "--- Etat du combat ---" << RESET << endl;
    cout << GREEN << player.getName() << " : HP " << player.getHp() << "/" << player.getHpMax() << RESET << endl;
    cout << RED << monster->getName() << " : HP " << monster->getHp() << "/" << monster->getHpMax() << RESET;
    cout << " | " << YELLOW << "Mercy : " << monster->getMercy() << "/" << monster->getMercyGoal() << RESET << endl;
    cout << BOLD << "----------------------" << RESET << endl;
}

void Combat::doFight() {
    int degats = rollDamage(monster->getHpMax(), player.getAtk() + player.getAtkBuff(), monster->getDef());

    if(degats == 0) {
        cout << GRAY << "Vous attaquez " << monster->getName() << " mais vous ratez votre coup !" << RESET << endl;
    } else {
        cout << RED << "Vous attaquez " << monster->getName() << " et infligez " << degats << " degats !" << RESET << endl;
        monster->takeDamage(degats);
        if(!monster->isAlive()) {
            cout << BOLD << RED << monster->getName() << " est vaincu !" << RESET << endl;
        }
    }
}

void Combat::doAct() {
    vector<string> acts = monster->getActIDs();
    int nbActs = monster->getNbActs();

    // on prend le min entre nbActs et la taille reelle du vecteur
    int nbDisponibles = nbActs;
    if(nbDisponibles > (int)acts.size()) nbDisponibles = (int)acts.size();

    // afficher les actions disponibles
    cout << CYAN << "Actions disponibles :" << RESET << endl;
    for(int i=0; i < nbDisponibles; i++) {
        cout << CYAN << i+1 << ". " << acts[i] << RESET << endl;
    }

    // choix du joueur
    int choix;
    cout << "Choisissez une action : ";
    while(!(cin >> choix) || choix < 1 || choix > nbDisponibles) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Choix invalide, reessayez : ";
    }

    string actID = acts[choix-1];

    // chercher l'action dans le catalogue
    if(actCatalog.find(actID) != actCatalog.end()) {
        ACTAction& action = actCatalog[actID];
        cout << endl << CYAN << action.text << RESET << endl;
        monster->addMercy(action.mercyImpact);

        if(action.mercyImpact > 0) {
            cout << YELLOW << "(Mercy +" << action.mercyImpact << ")" << RESET << endl;
        } else if(action.mercyImpact < 0) {
            cout << RED << "(Mercy " << action.mercyImpact << ")" << RESET << endl;
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
        cout << YELLOW << "Vous n'avez aucun item utilisable !" << RESET << endl;
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
    if(monster->canBeSpared()) {
        cout << BOLD << GREEN << "Vous epargnez " << monster->getName() << " !" << RESET << endl;
        cout << GREEN << monster->getName() << " s'en va paisiblement..." << RESET << endl;
        return true;
    } else {
        cout << YELLOW << "La jauge Mercy n'est pas assez haute ! (" << monster->getMercy() << "/" << monster->getMercyGoal() << ")" << RESET << endl;
        return false;
    }
}

void Combat::monsterTurn() {
    int degats = rollDamage(player.getHpMax(), monster->getAtk(), player.getDef() + player.getDefBuff());

    if(degats == 0) {
        cout << GRAY << monster->getName() << " vous attaque mais rate son coup !" << RESET << endl;
    } else {
        cout << RED << monster->getName() << " vous attaque et inflige " << degats << " degats !" << RESET << endl;
        player.takeDamage(degats);
    }
}

bool Combat::start() {
    cout << endl;
    cout << BOLD << MAGENTA << "==============================" << RESET << endl;
    cout << BOLD << MAGENTA << "Un " << monster->getName() << " (" << monster->getCategoryStr() << ") apparait !" << RESET << endl;
    cout << BOLD << MAGENTA << "==============================" << RESET << endl;

    bool spared = false;

    while(monster->isAlive() && player.isAlive() && !spared) {
        displayCombatStatus();

        // menu de combat
        cout << endl;
        cout << RED << "1. FIGHT" << RESET << endl;
        cout << CYAN << "2. ACT" << RESET << endl;
        cout << GREEN << "3. ITEM" << RESET << endl;
        cout << YELLOW << "4. MERCY" << RESET << endl;
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
        if(!monster->isAlive()) {
            player.addVictory();
            player.addKill();
            bestiary.addEntry(*monster, "Tue");
            cout << endl << BOLD << RED << "Vous avez tue " << monster->getName() << " !" << RESET << endl;
            cout << YELLOW << "Victoires : " << player.getVictories() << "/10" << RESET << endl;
            return true;
        }

        // si le monstre est epargne
        if(spared) {
            player.addVictory();
            player.addSpare();
            bestiary.addEntry(*monster, "Epargne");
            cout << endl << BOLD << GREEN << "Vous avez epargne " << monster->getName() << " !" << RESET << endl;
            cout << YELLOW << "Victoires : " << player.getVictories() << "/10" << RESET << endl;
            return true;
        }

        // tour du monstre (seulement si le tour a ete consomme)
        if(turnUsed) {
            cout << endl;
            monsterTurn();

            // verifier si le joueur est mort
            if(!player.isAlive()) {
                cout << endl << BOLD << RED << "Vous avez ete vaincu par " << monster->getName() << "..." << RESET << endl;
                cout << BOLD << RED << "GAME OVER" << RESET << endl;
                return false;
            }
        }
    }

    return true;
}
