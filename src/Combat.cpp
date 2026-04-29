#include "../include/Combat.h"
#include "../include/Colors.h"
#include "../include/Display.h"
#include <limits>
#include <sstream>

using namespace std;

Combat::Combat(Player& p, unique_ptr<Monster> m, map<string, ACTAction>& catalog,Bestiary& best, mt19937& rng)
    : player(p), monster(move(m)), actCatalog(catalog), bestiary(best), rng(rng) {
    // initialisation entièrement gérée par la liste d'initialisation
}

// formule de dégâts du sujet : tirage aléatoire entre 0 et hpMax, puis ajout de l'atk et soustraction de la def
// un tirage à 0 représente un coup raté ; sinon un minimum de 1 dégât est garanti
int Combat::rollDamage(int hpMax, int attackerAtk,int defenderDef) {
    uniform_int_distribution<int> dist(0, hpMax);
    int baseDamage = dist(rng);
    if(baseDamage == 0) return 0;       // coup raté
    int finalDamage = baseDamage + attackerAtk - defenderDef;
    if(finalDamage < 1) finalDamage = 1;    // garantit un dégât minimal pour qu'une grosse def n'annule pas tout
    if(finalDamage > hpMax) finalDamage = hpMax;    // les dégâts infligés ne peuvent pas dépasser les HP max de la cible
    return finalDamage;
}

void Combat::displayCombatStatus() {
    cout << endl;
    Display::drawTitleBox("etat du combat", 52, BOLD);
    Display::drawLeftLine(BOLD + player.getName() + RESET, 52, BOLD);
    Display::drawLeftLine(Display::hpBar("HP", player.getHp(), player.getHpMax()), 52, BOLD);
    Display::drawEmptyLine(52, BOLD);
    Display::drawLeftLine(BOLD + monster->getName() + RESET, 52, BOLD);
    Display::drawLeftLine(Display::hpBar("HP", monster->getHp(), monster->getHpMax()), 52, BOLD);
    Display::drawLeftLine(Display::mercyBar("MERCY", monster->getMercy(), monster->getMercyGoal()), 52, BOLD);
    Display::drawBottomBorder(52, BOLD);
}

void Combat::doFight() {
    // on intègre l'atkBuff temporaire (potion ou événement aléatoire) au calcul des dégâts
    int degats = rollDamage(monster->getHpMax(), player.getAtk() + player.getAtkBuff(), monster->getDef());

    if(degats == 0) {
        cout << "  " << GRAY << "tu attaques " << monster->getName() << " mais tu rates ton coup !" << RESET << endl;
    } else {
        cout << "  " << RED << "tu attaques " << monster->getName() << " et lui infliges " << degats << " degats !" << RESET << endl;
        monster->takeDamage(degats);
        if(!monster->isAlive()) {
            cout << "  " << BOLD << RED << monster->getName() << " est vaincu !" << RESET << endl;
        }
    }
}

void Combat::doAct() {
    vector<string> acts = monster->getActIDs();
    int nbActs = monster->getNbActs();

    // sécurité : on borne par la taille réelle du vecteur au cas où le csv déclare moins d'actions que la classe
    int nbDisponibles = nbActs;
    if(nbDisponibles > (int)acts.size()) nbDisponibles = (int)acts.size();

    Display::drawTitleBox("actions disponibles", 36, CYAN);
    for(int i=0; i < nbDisponibles; i++) {
        Display::drawMenuOption(i+1, acts[i], 36, CYAN);
    }
    Display::drawBottomBorder(36, CYAN);

    // saisie utilisateur avec validation des bornes
    int choix;
    cout << " choisis une action : ";
    while(!(cin >> choix) || choix < 1 || choix > nbDisponibles) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "choix invalide, recommence : ";
    }

    string actID = acts[choix-1];

    // résolution dans le catalogue ; en cas de csv mal formé on émet un message et on poursuit
    if(actCatalog.find(actID) != actCatalog.end()) {
        ACTAction& action = actCatalog[actID];
        cout << endl << "  " << CYAN << action.text << RESET << endl;
        monster->addMercy(action.mercyImpact);

        if(action.mercyImpact > 0) {
            cout << "  " << YELLOW << "(mercy +" << action.mercyImpact << ")" << RESET << endl;
        } else if(action.mercyImpact < 0) {
            cout << "  " << RED << "(mercy " << action.mercyImpact << ")" << RESET << endl;
        }
    } else {
        cout << "  action inconnue..." << endl;
    }
}

bool Combat::doItem() {
    vector<Item>& inv = player.getInventory();

    // vérifie l'existence d'au moins un item encore utilisable
    bool hasUsable = false;
    for(int i=0; i < (int)inv.size(); i++) {
        if(inv[i].isUsable()) hasUsable = true;
    }

    if(!hasUsable) {
        cout << "  " << YELLOW << "tu n'as aucun item utilisable !" << RESET << endl;
        return false;       // le tour n'est pas consommé
    }

    player.displayItems();

    int choix;
    cout << "choisis un item (0 pour annuler) : ";
    while(!(cin >> choix) || choix < 0 || choix > (int)inv.size()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "choix invalide, recommence : ";
    }

    if(choix == 0) return false;        // annulation : on permet de rejouer ce tour

    return player.useItem(choix-1);
}

bool Combat::doMercy() {
    // l'épargne n'est possible que si la jauge mercy a atteint son seuil
    if(monster->canBeSpared()) {
        cout << "  " << BOLD << GREEN << "tu epargnes " << monster->getName() << " !" << RESET << endl;
        cout << "  " << GREEN << monster->getName() << " s'en va paisiblement..." << RESET << endl;
        return true;
    } else {
        cout << "  " << YELLOW << "la jauge mercy n'est pas assez haute ! (" << monster->getMercy() << "/" << monster->getMercyGoal() << ")" << RESET << endl;
        return false;
    }
}

void Combat::monsterTurn() {
    // tour d'attaque du monstre, en intégrant le defBuff temporaire du joueur
    int degats = rollDamage(player.getHpMax(), monster->getAtk(), player.getDef() + player.getDefBuff());

    // multiplicateur de degats selon la categorie du monstre
    if(degats > 0) {
        double multiplier = 1.0;
        switch(monster->getCategory()) {
            case MINIBOSS: multiplier = 1.3; break;
            case BOSS:     multiplier = 1.6; break;
            default:       break;
        }
        degats = (int)(degats * multiplier);
        if(degats < 1) degats = 1;
        if(degats > player.getHpMax()) degats = player.getHpMax();    // cap après multiplicateur miniboss/boss
    }

    if(degats == 0) {
        cout << "  " << GRAY << monster->getName() << " t'attaque mais rate son coup !" << RESET << endl;
    } else {
        cout << "  " << RED << monster->getName() << " t'attaque et inflige " << degats << " degats !" << RESET << endl;
        player.takeDamage(degats);
    }
}

bool Combat::start() {
    cout << endl;
    Display::drawTopBorder(52, BOLD + MAGENTA);
    Display::drawCenteredLine(BOLD + MAGENTA + "* COMBAT *" + RESET, 52, BOLD + MAGENTA);
    Display::drawSeparator(52, BOLD + MAGENTA);
    ostringstream encounterLine;
    encounterLine << "un " << monster->getName() << " (" << monster->getCategoryStr() << ") apparait !";
    Display::drawCenteredLine(encounterLine.str(), 52, BOLD + MAGENTA);
    Display::drawBottomBorder(52, BOLD + MAGENTA);

    bool spared = false;

    // boucle principale du combat : continue tant que les deux camps sont en vie et qu'aucune épargne n'a eu lieu
    while(monster->isAlive() && player.isAlive() && !spared) {
        displayCombatStatus();

        cout << endl;
        Display::drawTopBorder(28, "");
        Display::drawMenuOption(1, "fight", 28, RED);
        Display::drawMenuOption(2, "act", 28, CYAN);
        Display::drawMenuOption(3, "item", 28, GREEN);
        Display::drawMenuOption(4, "mercy", 28, YELLOW);
        Display::drawBottomBorder(28, "");
        cout << " que veux-tu faire ? ";

        int choix;
        while(!(cin >> choix) || choix < 1 || choix > 4) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << " choix invalide, recommence : ";
        }

        // si turnUsed reste à false, le tour n'est pas consommé et le joueur peut rejouer
        bool turnUsed = true;

        switch(choix) {
            case 1:
                doFight();
                break;
            case 2:
                doAct();
                break;
            case 3:
                if(!doItem()) {
                    turnUsed = false;       // aucun item utilisé : on rejoue le tour
                }
                break;
            case 4:
                if(doMercy()) {
                    spared = true;
                } else {
                    turnUsed = false;       // mercy insuffisante : on rejoue le tour
                }
                break;
        }

        // monstre vaincu : enregistrement de la victoire et sortie de la boucle
        if(!monster->isAlive()) {
            player.addVictory();
            player.addKill();
            bestiary.addEntry(*monster, "Tue");
            cout << endl;
            Display::drawTopBorder(40, RED);
            ostringstream killMsg;
            killMsg << BOLD << RED << "tu as tue " << monster->getName() << " !" << RESET;
            Display::drawCenteredLine(killMsg.str(), 40, RED);
            ostringstream victMsg;
            victMsg << YELLOW << "victoires : " << player.getVictories() << "/10" << RESET;
            Display::drawCenteredLine(victMsg.str(), 40, RED);
            Display::drawBottomBorder(40, RED);
            return true;
        }

        // monstre épargné : enregistrement de la victoire pacifiste et sortie de la boucle
        if(spared) {
            player.addVictory();
            player.addSpare();
            bestiary.addEntry(*monster, "Epargne");
            cout << endl;
            Display::drawTopBorder(40, GREEN);
            ostringstream spareMsg;
            spareMsg << BOLD << GREEN << "tu as epargne " << monster->getName() << " !" << RESET;
            Display::drawCenteredLine(spareMsg.str(), 40, GREEN);
            ostringstream victMsg2;
            victMsg2 << YELLOW << "victoires : " << player.getVictories() << "/10" << RESET;
            Display::drawCenteredLine(victMsg2.str(), 40, GREEN);
            Display::drawBottomBorder(40, GREEN);
            return true;
        }

        // le monstre ne joue son tour que si l'action du joueur a effectivement consommé son propre tour
        if(turnUsed) {
            cout << endl;
            monsterTurn();

            if(!player.isAlive()) {
                cout << endl;
                Display::drawTopBorder(40, RED);
                ostringstream defeatMsg;
                defeatMsg << BOLD << RED << "tu as ete vaincu par " << monster->getName() << "..." << RESET;
                Display::drawCenteredLine(defeatMsg.str(), 40, RED);
                Display::drawCenteredLine(BOLD + RED + "GAME OVER" + RESET, 40, RED);
                Display::drawBottomBorder(40, RED);
                return false;
            }
        }
    }

    // valeur de retour par défaut : le joueur survit (cas notamment de l'épargne)
    return true;
}
