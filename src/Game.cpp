#include "../include/Game.h"
#include "../include/Colors.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

using namespace std;

Game::Game() : rng(random_device{}()) {
    gameOver = false;
    initActCatalog();
    initEventCatalog();
}

void Game::initActCatalog() {
    // on remplit le catalogue avec les 10 actions ACT
    ACTAction a;

    a.id = "COMPLIMENT"; a.text = "Tu as de tres beaux yeux... pour un monstre."; a.mercyImpact = 15;
    actCatalog["COMPLIMENT"] = a;

    a.id = "JOKE"; a.text = "Pourquoi les monstres ne font pas de maths ? Parce qu'ils ont peur des problemes !"; a.mercyImpact = 10;
    actCatalog["JOKE"] = a;

    a.id = "DISCUSS"; a.text = "Vous discutez du beau temps... il pleut des epees."; a.mercyImpact = 5;
    actCatalog["DISCUSS"] = a;

    a.id = "DANCE"; a.text = "Vous faites la Macarena. Le monstre est perplexe."; a.mercyImpact = 20;
    actCatalog["DANCE"] = a;

    a.id = "PET"; a.text = "Vous caressez le monstre. Il ronronne... bizarrement."; a.mercyImpact = 15;
    actCatalog["PET"] = a;

    a.id = "OBSERVE"; a.text = "Vous observez le monstre attentivement. Il rougit."; a.mercyImpact = 10;
    actCatalog["OBSERVE"] = a;

    a.id = "OFFER_SNACK"; a.text = "Vous offrez un snack. Le monstre l'engloutit d'un coup."; a.mercyImpact = 10;
    actCatalog["OFFER_SNACK"] = a;

    a.id = "REASON"; a.text = "Vous tentez de raisonner le monstre. Il semble reflechir."; a.mercyImpact = 20;
    actCatalog["REASON"] = a;

    a.id = "INSULT"; a.text = "Vous insultez le monstre. Il est furieux !"; a.mercyImpact = -20;
    actCatalog["INSULT"] = a;

    a.id = "THREATEN"; a.text = "Vous menacez le monstre. L'ambiance est glaciale."; a.mercyImpact = -15;
    actCatalog["THREATEN"] = a;
}

void Game::initEventCatalog() {
    // on remplit le catalogue avec 8 evenements aleatoires
    RandomEvent e;

    e.name = "Source miraculeuse"; e.description = "Vous trouvez une source miraculeuse et recuperez des HP !"; e.type = "HEAL"; e.value = 20;
    eventCatalog.push_back(e);

    e.name = "Piege a ours"; e.description = "Vous marchez sur un piege ! Vous perdez des HP !"; e.type = "DAMAGE"; e.value = 15;
    eventCatalog.push_back(e);

    e.name = "Epee rouillee"; e.description = "Vous trouvez une epee rouillee. Votre ATK augmente temporairement !"; e.type = "ATK_BOOST"; e.value = 3;
    eventCatalog.push_back(e);

    e.name = "Bouclier fissure"; e.description = "Vous trouvez un bouclier fissure. Votre DEF augmente temporairement !"; e.type = "DEF_BOOST"; e.value = 2;
    eventCatalog.push_back(e);

    e.name = "Marchand ambulant"; e.description = "Un marchand vous offre un Snack !"; e.type = "ITEM_FIND"; e.value = 0;
    eventCatalog.push_back(e);

    e.name = "Embuscade"; e.description = "Des cailloux tombent du ciel ! Vous perdez des HP !"; e.type = "DAMAGE"; e.value = 10;
    eventCatalog.push_back(e);

    e.name = "Meditation"; e.description = "Vous meditez un instant. Vous recuperez quelques HP."; e.type = "HEAL"; e.value = 10;
    eventCatalog.push_back(e);

    e.name = "Benediction"; e.description = "Une lumiere vous enveloppe. Votre ATK augmente !"; e.type = "ATK_BOOST"; e.value = 2;
    eventCatalog.push_back(e);
}

void Game::triggerRandomEvent() {
    // 40% de chance de declencher un evenement
    uniform_int_distribution<int> chanceDist(1, 100);
    if(chanceDist(rng) > 40) return;

    // tirer un evenement au hasard
    uniform_int_distribution<int> eventDist(0, eventCatalog.size()-1);
    RandomEvent& evt = eventCatalog[eventDist(rng)];

    cout << endl;
    cout << BOLD << MAGENTA << "*** EVENEMENT : " << evt.name << " ***" << RESET << endl;
    cout << CYAN << evt.description << RESET << endl;

    if(evt.type == "HEAL") {
        player.heal(evt.value);
        cout << GREEN << "HP restaures : +" << evt.value << " (HP : " << player.getHp() << "/" << player.getHpMax() << ")" << RESET << endl;
    } else if(evt.type == "DAMAGE") {
        player.takeDamage(evt.value);
        cout << RED << "Degats subis : " << evt.value << " (HP : " << player.getHp() << "/" << player.getHpMax() << ")" << RESET << endl;
        if(!player.isAlive()) {
            cout << BOLD << RED << "Vous avez succombe a l'evenement... GAME OVER" << RESET << endl;
            gameOver = true;
        }
    } else if(evt.type == "ATK_BOOST") {
        player.setAtkBuff(player.getAtkBuff() + evt.value);
        cout << CYAN << "ATK temporairement +" << evt.value << " !" << RESET << endl;
    } else if(evt.type == "DEF_BOOST") {
        player.setDefBuff(player.getDefBuff() + evt.value);
        cout << CYAN << "DEF temporairement +" << evt.value << " !" << RESET << endl;
    } else if(evt.type == "ITEM_FIND") {
        Item snack("Snack", "HEAL", 8, 1);
        player.addItem(snack);
        cout << GREEN << "Vous obtenez un Snack !" << RESET << endl;
    }
}

MonsterCategory Game::parseCategory(string cat) {
    if(cat == "MINIBOSS") return MINIBOSS;
    if(cat == "BOSS") return BOSS;
    return NORMAL;
}

bool Game::loadItems(string filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        cout << "Erreur : fichier " << filename << " introuvable." << endl;
        return false;
    }

    string line;
    while(getline(file, line)) {
        // enlever le \r si on est sur Windows
        if(!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if(line.empty()) continue;

        stringstream ss(line);
        string nom, type, valStr, qteStr;

        // on decoupe avec le separateur ;
        if(!getline(ss, nom, ';')) continue;
        if(!getline(ss, type, ';')) continue;
        if(!getline(ss, valStr, ';')) continue;
        if(!getline(ss, qteStr, ';')) continue;

        try {
            int val = stoi(valStr);
            int qte = stoi(qteStr);
            Item item(nom, type, val, qte);
            player.addItem(item);
        } catch(...) {
            cout << "Attention : ligne mal formee dans " << filename << " : " << line << endl;
        }
    }

    file.close();
    return true;
}

bool Game::loadMonsters(string filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        cout << "Erreur : fichier " << filename << " introuvable." << endl;
        return false;
    }

    string line;
    while(getline(file, line)) {
        // enlever le \r si on est sur Windows
        if(!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if(line.empty()) continue;

        stringstream ss(line);
        string catStr, nom, hpStr, atkStr, defStr, mercyStr;
        string act1, act2, act3, act4;

        // on decoupe la ligne
        if(!getline(ss, catStr, ';')) continue;
        if(!getline(ss, nom, ';')) continue;
        if(!getline(ss, hpStr, ';')) continue;
        if(!getline(ss, atkStr, ';')) continue;
        if(!getline(ss, defStr, ';')) continue;
        if(!getline(ss, mercyStr, ';')) continue;
        if(!getline(ss, act1, ';')) continue;
        if(!getline(ss, act2, ';')) continue;
        if(!getline(ss, act3, ';')) continue;
        if(!getline(ss, act4)) continue; // le dernier champ n'a pas de ; apres

        try {
            int hp = stoi(hpStr);
            int atk = stoi(atkStr);
            int def = stoi(defStr);
            int mercyGoal = stoi(mercyStr);

            MonsterCategory cat = parseCategory(catStr);

            // on recupere les actions valides (pas les "-")
            vector<string> acts;
            if(act1 != "-") {
                if(actCatalog.find(act1) != actCatalog.end()) {
                    acts.push_back(act1);
                } else {
                    cout << "Attention : action ACT '" << act1 << "' inconnue pour " << nom << endl;
                }
            }
            if(act2 != "-") {
                if(actCatalog.find(act2) != actCatalog.end()) {
                    acts.push_back(act2);
                } else {
                    cout << "Attention : action ACT '" << act2 << "' inconnue pour " << nom << endl;
                }
            }
            if(act3 != "-") {
                if(actCatalog.find(act3) != actCatalog.end()) {
                    acts.push_back(act3);
                } else {
                    cout << "Attention : action ACT '" << act3 << "' inconnue pour " << nom << endl;
                }
            }
            if(act4 != "-") {
                if(actCatalog.find(act4) != actCatalog.end()) {
                    acts.push_back(act4);
                } else {
                    cout << "Attention : action ACT '" << act4 << "' inconnue pour " << nom << endl;
                }
            }

            // factory : on cree la bonne sous-classe selon la categorie
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
            cout << "Attention : ligne mal formee dans " << filename << " : " << line << endl;
        }
    }

    file.close();
    return true;
}

void Game::startCombat() {
    if(monsterPool.empty()) {
        cout << "Aucun monstre disponible !" << endl;
        return;
    }

    // tirer un monstre au hasard
    uniform_int_distribution<int> dist(0, monsterPool.size()-1);
    int index = dist(rng);

    // cloner le monstre et le remettre a zero (polymorphisme)
    unique_ptr<Monster> m = monsterPool[index]->clone();
    m->resetForCombat();

    // lancer le combat
    Combat combat(player, move(m), actCatalog, bestiary, rng);
    bool survived = combat.start();

    // reset les buffs temporaires apres chaque combat
    player.resetBuffs();

    if(!survived) {
        gameOver = true;
    } else if(!gameOver) {
        // evenement aleatoire apres un combat gagne
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

    // proposer d'utiliser un item
    vector<Item>& inv = player.getInventory();
    bool hasUsable = false;
    for(int i=0; i < (int)inv.size(); i++) {
        if(inv[i].isUsable()) hasUsable = true;
    }

    if(!hasUsable) return;

    cout << endl << "Voulez-vous utiliser un item ? (o/n) : ";
    char rep;
    cin >> rep;

    if(rep == 'o' || rep == 'O') {
        cout << "Quel item ? (numero) : ";
        int choix;
        while(!(cin >> choix) || choix < 1 || choix > (int)inv.size()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Choix invalide, reessayez : ";
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
    cout << BOLD << YELLOW << "   FELICITATIONS ! 10 victoires atteintes !" << RESET << endl;
    cout << BOLD << YELLOW << "============================================" << RESET << endl;
    cout << endl;

    int killed = player.getMonstersKilled();
    int spared = player.getMonstersSpared();

    if(spared == 0) {
        // fin genocidaire
        cout << BOLD << RED << "=== FIN GENOCIDAIRE ===" << RESET << endl;
        cout << RED << "Vous avez tue tous les monstres sans exception." << RESET << endl;
        cout << RED << "Le monde tremble devant votre puissance..." << RESET << endl;
        cout << RED << "Mais a quel prix ?" << RESET << endl;
    } else if(killed == 0) {
        // fin pacifiste
        cout << BOLD << GREEN << "=== FIN PACIFISTE ===" << RESET << endl;
        cout << GREEN << "Vous avez epargne chaque monstre que vous avez croise." << RESET << endl;
        cout << GREEN << "Le monde est en paix grace a votre compassion." << RESET << endl;
        cout << GREEN << "Les monstres vous considerent comme un ami." << RESET << endl;
    } else {
        // fin neutre
        cout << BOLD << YELLOW << "=== FIN NEUTRE ===" << RESET << endl;
        cout << YELLOW << "Vous avez tue " << killed << " monstre(s) et epargne " << spared << " monstre(s)." << RESET << endl;
        cout << YELLOW << "Votre chemin fut un melange de violence et de compassion." << RESET << endl;
        cout << YELLOW << "Le monde ne sait pas trop quoi penser de vous..." << RESET << endl;
    }

    cout << endl;
    cout << BOLD << CYAN << "Merci d'avoir joue a ALTERDUNE !" << RESET << endl;
}

void Game::showMenu() {
    cout << endl;
    cout << BOLD << BLUE << "===== MENU PRINCIPAL =====" << RESET << endl;
    cout << CYAN << "1. Bestiaire" << RESET << endl;
    cout << CYAN << "2. Demarrer un combat" << RESET << endl;
    cout << CYAN << "3. Statistiques" << RESET << endl;
    cout << CYAN << "4. Items" << RESET << endl;
    cout << CYAN << "5. Quitter" << RESET << endl;
    cout << BOLD << BLUE << "==========================" << RESET << endl;
    cout << "Votre choix : ";
}

void Game::run() {
    cout << BOLD << CYAN << "========================================" << RESET << endl;
    cout << BOLD << CYAN << "         BIENVENUE DANS ALTERDUNE       " << RESET << endl;
    cout << BOLD << CYAN << "========================================" << RESET << endl;
    cout << endl;

    // etape 1 : saisie du nom
    cout << "Entrez le nom de votre personnage : ";
    string nom;
    getline(cin, nom);
    player = Player(nom);

    // etape 2 : chargement des fichiers
    if(!loadItems("items.csv")) {
        return;
    }
    if(!loadMonsters("monsters.csv")) {
        return;
    }

    // etape 3 : affichage du resume
    cout << endl;
    cout << "--- Recapitulatif ---" << endl;
    cout << "Joueur : " << player.getName() << endl;
    cout << "HP : " << player.getHp() << "/" << player.getHpMax() << endl;
    cout << "Items :" << endl;
    player.displayItems();
    cout << endl;
    cout << monsterPool.size() << " monstre(s) charge(s)." << endl;

    // etape 4 : boucle du menu principal
    while(player.getVictories() < 10 && !gameOver) {
        showMenu();

        int choix;
        while(!(cin >> choix) || choix < 1 || choix > 5) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Choix invalide, reessayez : ";
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
                cout << "Au revoir !" << endl;
                return;
        }
    }

    // si on arrive ici avec 10 victoires
    if(player.getVictories() >= 10) {
        checkEnding();
    }
}
