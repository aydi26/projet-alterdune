#include "../include/Game.h"
#include "../include/Colors.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

using namespace std;

// le générateur aléatoire est initialisé une seule fois ici puis transmis à Combat
Game::Game() : rng(random_device{}()) {
    gameOver = false;
    initActCatalog();
    initEventCatalog();
}

void Game::initActCatalog() {
    // initialisation en dur des 10 actions ACT du jeu
    // un chargement depuis fichier serait possible mais pas requis par le sujet
    ACTAction a;

    a.id = "COMPLIMENT"; a.text = "tu as de tres beaux yeux... pour un monstre."; a.mercyImpact = 15;
    actCatalog["COMPLIMENT"] = a;

    a.id = "JOKE"; a.text = "pourquoi les monstres ne font pas de maths ? parce qu'ils ont peur des problemes !"; a.mercyImpact = 10;
    actCatalog["JOKE"] = a;

    a.id = "DISCUSS"; a.text = "vous discutez du beau temps... il pleut des epees."; a.mercyImpact = 5;
    actCatalog["DISCUSS"] = a;

    a.id = "DANCE"; a.text = "vous faites la macarena. le monstre est perplexe."; a.mercyImpact = 20;
    actCatalog["DANCE"] = a;

    a.id = "PET"; a.text = "tu caresses le monstre. il ronronne... bizarrement."; a.mercyImpact = 15;
    actCatalog["PET"] = a;

    a.id = "OBSERVE"; a.text = "tu observes le monstre attentivement. il rougit."; a.mercyImpact = 10;
    actCatalog["OBSERVE"] = a;

    a.id = "OFFER_SNACK"; a.text = "tu offres un snack. le monstre l'engloutit d'un coup."; a.mercyImpact = 10;
    actCatalog["OFFER_SNACK"] = a;

    a.id = "REASON"; a.text = "tu tentes de raisonner le monstre. il semble reflechir."; a.mercyImpact = 20;
    actCatalog["REASON"] = a;

    // les deux actions suivantes ont un impact négatif et permettent de compromettre une route pacifiste
    a.id = "INSULT"; a.text = "tu insultes le monstre. il est furieux !"; a.mercyImpact = -20;
    actCatalog["INSULT"] = a;

    a.id = "THREATEN"; a.text = "tu menaces le monstre. l'ambiance est glaciale."; a.mercyImpact = -15;
    actCatalog["THREATEN"] = a;
}

void Game::initEventCatalog() {
    // catalogue de 8 événements aléatoires pouvant être déclenchés entre deux combats
    RandomEvent e;

    e.name = "source miraculeuse"; e.description = "tu trouves une source miraculeuse et tu recuperes des hp !"; e.type = "HEAL"; e.value = 20;
    eventCatalog.push_back(e);

    e.name = "piege a ours"; e.description = "tu marches sur un piege ! tu perds des hp !"; e.type = "DAMAGE"; e.value = 15;
    eventCatalog.push_back(e);

    e.name = "epee rouillee"; e.description = "tu trouves une epee rouillee. ton atk augmente temporairement !"; e.type = "ATK_BOOST"; e.value = 3;
    eventCatalog.push_back(e);

    e.name = "bouclier fissure"; e.description = "tu trouves un bouclier fissure. ta def augmente temporairement !"; e.type = "DEF_BOOST"; e.value = 2;
    eventCatalog.push_back(e);

    e.name = "marchand ambulant"; e.description = "un marchand t'offre un snack !"; e.type = "ITEM_FIND"; e.value = 0;
    eventCatalog.push_back(e);

    e.name = "embuscade"; e.description = "des cailloux tombent du ciel ! tu perds des hp !"; e.type = "DAMAGE"; e.value = 10;
    eventCatalog.push_back(e);

    e.name = "meditation"; e.description = "tu medites un instant. tu recuperes quelques hp."; e.type = "HEAL"; e.value = 10;
    eventCatalog.push_back(e);

    e.name = "benediction"; e.description = "une lumiere t'enveloppe. ton atk augmente !"; e.type = "ATK_BOOST"; e.value = 2;
    eventCatalog.push_back(e);
}

void Game::triggerRandomEvent() {
    // probabilité de déclenchement fixée à 40 % afin d'éviter une fréquence d'événements trop élevée
    uniform_int_distribution<int> chanceDist(1, 100);
    if(chanceDist(rng) > 40) return;

    // tirage uniforme d'un événement parmi le catalogue
    uniform_int_distribution<int> eventDist(0, eventCatalog.size()-1);
    RandomEvent& evt = eventCatalog[eventDist(rng)];

    cout << endl;
    cout << BOLD << MAGENTA << "*** evenement : " << evt.name << " ***" << RESET << endl;
    cout << CYAN << evt.description << RESET << endl;

    // dispatch sur le type d'événement via une chaîne de if/else, suffisante pour le nombre de cas couverts
    if(evt.type == "HEAL") {
        player.heal(evt.value);
        cout << GREEN << "hp restaures : +" << evt.value << " (hp : " << player.getHp() << "/" << player.getHpMax() << ")" << RESET << endl;
    } else if(evt.type == "DAMAGE") {
        player.takeDamage(evt.value);
        cout << RED << "degats subis : " << evt.value << " (hp : " << player.getHp() << "/" << player.getHpMax() << ")" << RESET << endl;
        // un événement peut être létal : on doit donc gérer le cas du game over ici aussi
        if(!player.isAlive()) {
            cout << BOLD << RED << "tu as succombe a l'evenement... game over" << RESET << endl;
            gameOver = true;
        }
    } else if(evt.type == "ATK_BOOST") {
        player.setAtkBuff(player.getAtkBuff() + evt.value);
        cout << CYAN << "atk temporairement +" << evt.value << " !" << RESET << endl;
    } else if(evt.type == "DEF_BOOST") {
        player.setDefBuff(player.getDefBuff() + evt.value);
        cout << CYAN << "def temporairement +" << evt.value << " !" << RESET << endl;
    } else if(evt.type == "ITEM_FIND") {
        // un seul item est attribué pour le moment ; le pool pourra être étendu ultérieurement
        Item snack("Snack", "HEAL", 8, 1);
        player.addItem(snack);
        cout << GREEN << "tu obtiens un snack !" << RESET << endl;
    }
}

MonsterCategory Game::parseCategory(string cat) {
    if(cat == "MINIBOSS") return MINIBOSS;
    if(cat == "BOSS") return BOSS;
    return NORMAL;      // valeur de repli pour toute chaîne non reconnue
}

bool Game::loadItems(string filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        cout << "erreur : fichier " << filename << " introuvable." << endl;
        return false;
    }

    string line;
    while(getline(file, line)) {
        // suppression du retour chariot final présent sur les fichiers édités sous Windows
        if(!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if(line.empty()) continue;

        stringstream ss(line);
        string nom, type, valStr, qteStr;

        // découpage de la ligne sur le séparateur ';'
        if(!getline(ss, nom, ';')) continue;
        if(!getline(ss, type, ';')) continue;
        if(!getline(ss, valStr, ';')) continue;
        if(!getline(ss, qteStr, ';')) continue;

        // try/catch nécessaire car stoi lève une exception en cas de chaîne non numérique
        try {
            int val = stoi(valStr);
            int qte = stoi(qteStr);
            Item item(nom, type, val, qte);
            player.addItem(item);
        } catch(...) {
            cout << "attention : ligne mal formee dans " << filename << " : " << line << endl;
        }
    }

    file.close();
    return true;
}

bool Game::loadMonsters(string filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        cout << "erreur : fichier " << filename << " introuvable." << endl;
        return false;
    }

    string line;
    while(getline(file, line)) {
        // même traitement que pour les items pour le retour chariot Windows
        if(!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if(line.empty()) continue;

        stringstream ss(line);
        string catStr, nom, hpStr, atkStr, defStr, mercyStr;
        string act1, act2, act3, act4;

        // découpage explicite de chaque champ ; verbeux mais lisible pour ce parsing csv
        if(!getline(ss, catStr, ';')) continue;
        if(!getline(ss, nom, ';')) continue;
        if(!getline(ss, hpStr, ';')) continue;
        if(!getline(ss, atkStr, ';')) continue;
        if(!getline(ss, defStr, ';')) continue;
        if(!getline(ss, mercyStr, ';')) continue;
        if(!getline(ss, act1, ';')) continue;
        if(!getline(ss, act2, ';')) continue;
        if(!getline(ss, act3, ';')) continue;
        if(!getline(ss, act4)) continue;        // dernière colonne : pas de séparateur final

        try {
            int hp = stoi(hpStr);
            int atk = stoi(atkStr);
            int def = stoi(defStr);
            int mercyGoal = stoi(mercyStr);

            MonsterCategory cat = parseCategory(catStr);

            // on ne conserve que les actions différentes de "-" et présentes dans le catalogue
            // les actions inconnues génèrent un avertissement non bloquant pour ne pas interrompre le chargement
            vector<string> acts;
            if(act1 != "-") {
                if(actCatalog.find(act1) != actCatalog.end()) {
                    acts.push_back(act1);
                } else {
                    cout << "attention : action act '" << act1 << "' inconnue pour " << nom << endl;
                }
            }
            if(act2 != "-") {
                if(actCatalog.find(act2) != actCatalog.end()) {
                    acts.push_back(act2);
                } else {
                    cout << "attention : action act '" << act2 << "' inconnue pour " << nom << endl;
                }
            }
            if(act3 != "-") {
                if(actCatalog.find(act3) != actCatalog.end()) {
                    acts.push_back(act3);
                } else {
                    cout << "attention : action act '" << act3 << "' inconnue pour " << nom << endl;
                }
            }
            if(act4 != "-") {
                if(actCatalog.find(act4) != actCatalog.end()) {
                    acts.push_back(act4);
                } else {
                    cout << "attention : action act '" << act4 << "' inconnue pour " << nom << endl;
                }
            }

            // factory : instanciation de la sous-classe correspondant à la catégorie lue
            // point clé du polymorphisme : le reste du code manipule ensuite un Monster*
            unique_ptr<Monster> m;
            if(cat == NORMAL) {
                m = make_unique<MonsterNormal>(nom, hp, atk, def, mercyGoal, acts);
            } else if(cat == MINIBOSS) {
                m = make_unique<MonsterMiniBoss>(nom, hp, atk, def, mercyGoal, acts);
            } else {
                m = make_unique<MonsterBoss>(nom, hp, atk, def, mercyGoal, acts);
            }
            monsterPool.push_back(move(m));

        } catch(...) {
            cout << "attention : ligne mal formee dans " << filename << " : " << line << endl;
        }
    }

    file.close();
    return true;
}

void Game::startCombat() {
    if(monsterPool.empty()) {
        cout << "aucun monstre disponible !" << endl;
        return;
    }

    // sélection aléatoire d'un monstre dans le pool
    uniform_int_distribution<int> dist(0, monsterPool.size()-1);
    int index = dist(rng);

    // on travaille sur un clone pour préserver l'instance originale entre les combats successifs
    unique_ptr<Monster> m = monsterPool[index]->clone();
    m->resetForCombat();

    // l'intégralité du déroulement est déléguée à la classe Combat
    Combat combat(player, move(m), actCatalog, bestiary, rng);
    bool survived = combat.start();

    // les buffs sont temporaires : ils sont systématiquement remis à zéro après un combat
    player.resetBuffs();

    if(!survived) {
        gameOver = true;
    } else if(!gameOver) {
        // après une victoire, on tente de déclencher un événement aléatoire
        triggerRandomEvent();
    }
}

void Game::showStats() {
    cout << endl;
    player.displayInfo();
    cout << endl;
}

void Game::showItems() {
    cout << endl;
    player.displayItems();

    // si l'inventaire contient au moins un item utilisable, on propose de l'utiliser immédiatement
    vector<Item>& inv = player.getInventory();
    bool hasUsable = false;
    for(int i=0; i < (int)inv.size(); i++) {
        if(inv[i].isUsable()) hasUsable = true;
    }

    if(!hasUsable) return;

    cout << endl << "veux-tu utiliser un item ? (o/n) : ";
    char rep;
    cin >> rep;

    if(rep == 'o' || rep == 'O') {
        cout << "quel item ? (numero) : ";
        int choix;
        while(!(cin >> choix) || choix < 1 || choix > (int)inv.size()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "choix invalide, recommence : ";
        }
        player.useItem(choix-1);
    }
}

void Game::showBestiary() {
    cout << endl;
    bestiary.display();
}

void Game::checkEnding() {
    cout << endl;
    cout << BOLD << YELLOW << "============================================" << RESET << endl;
    cout << BOLD << YELLOW << "   felicitations ! 10 victoires atteintes !" << RESET << endl;
    cout << BOLD << YELLOW << "============================================" << RESET << endl;
    cout << endl;

    int killed = player.getMonstersKilled();
    int spared = player.getMonstersSpared();

    // trois fins possibles, déterminées par le profil de jeu :
    //  - aucun monstre épargné  -> fin génocidaire
    //  - aucun monstre tué      -> fin pacifiste
    //  - les deux               -> fin neutre
    if(spared == 0) {
        cout << BOLD << RED << "=== fin genocidaire ===" << RESET << endl;
        cout << RED << "tu as tue tous les monstres sans exception." << RESET << endl;
        cout << RED << "le monde tremble devant ta puissance..." << RESET << endl;
        cout << RED << "mais a quel prix ?" << RESET << endl;
    } else if(killed == 0) {
        cout << BOLD << GREEN << "=== fin pacifiste ===" << RESET << endl;
        cout << GREEN << "tu as epargne chaque monstre que tu as croise." << RESET << endl;
        cout << GREEN << "le monde est en paix grace a ta compassion." << RESET << endl;
        cout << GREEN << "les monstres te considerent comme un ami." << RESET << endl;
    } else {
        cout << BOLD << YELLOW << "=== fin neutre ===" << RESET << endl;
        cout << YELLOW << "tu as tue " << killed << " monstre(s) et epargne " << spared << " monstre(s)." << RESET << endl;
        cout << YELLOW << "ton chemin fut un melange de violence et de compassion." << RESET << endl;
        cout << YELLOW << "le monde ne sait pas trop quoi penser de toi..." << RESET << endl;
    }

    cout << endl;
    cout << BOLD << CYAN << "merci d'avoir joue a alterdune !" << RESET << endl;
}

void Game::showMenu() {
    cout << endl;
    cout << BOLD << BLUE << "===== menu principal =====" << RESET << endl;
    cout << CYAN << "1. bestiaire" << RESET << endl;
    cout << CYAN << "2. demarrer un combat" << RESET << endl;
    cout << CYAN << "3. statistiques" << RESET << endl;
    cout << CYAN << "4. items" << RESET << endl;
    cout << CYAN << "5. quitter" << RESET << endl;
    cout << BOLD << BLUE << "==========================" << RESET << endl;
    cout << "ton choix : ";
}

void Game::run() {
    cout << BOLD << CYAN << "========================================" << RESET << endl;
    cout << BOLD << CYAN << "         bienvenue dans alterdune       " << RESET << endl;
    cout << BOLD << CYAN << "========================================" << RESET << endl;
    cout << endl;

    // étape 1 : saisie du nom du personnage
    cout << "entre le nom de ton personnage : ";
    string nom;
    getline(cin, nom);
    player = Player(nom);

    // étape 2 : chargement des données ; on interrompt l'exécution si l'un des fichiers est introuvable
    if(!loadItems("items.csv")) {
        return;
    }
    if(!loadMonsters("monsters.csv")) {
        return;
    }

    // étape 3 : récapitulatif pour confirmer le bon chargement de l'état initial
    cout << endl;
    cout << "--- recapitulatif ---" << endl;
    cout << "joueur : " << player.getName() << endl;
    cout << "hp : " << player.getHp() << "/" << player.getHpMax() << endl;
    cout << "items :" << endl;
    player.displayItems();
    cout << endl;
    cout << monsterPool.size() << " monstre(s) charge(s)." << endl;

    // étape 4 : boucle principale du menu, jusqu'à 10 victoires ou game over
    while(player.getVictories() < 10 && !gameOver) {
        showMenu();

        int choix;
        while(!(cin >> choix) || choix < 1 || choix > 5) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "choix invalide, recommence : ";
        }

        switch(choix) {
            case 1:
                showBestiary();
                break;
            case 2:
                startCombat();
                break;
            case 3:
                showStats();
                break;
            case 4:
                showItems();
                break;
            case 5:
                cout << "a plus !" << endl;
                return;
        }
    }

    // sortie de la boucle avec >= 10 victoires : la partie est terminée, on affiche l'écran de fin
    if(player.getVictories() >= 10) {
        checkEnding();
    }
}
