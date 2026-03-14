# ALTERDUNE — Mini-RPG en C++ (Console)

> Projet de Programmation Orientee Objet — ESILV — Second semestre 2025-2026  
> Encadrant : Daniel Wladdimiro (`daniel.wladdimiro@devinci.fr`)

---

## Table des matieres

1. [Presentation du projet](#presentation-du-projet)
2. [Objectifs pedagogiques](#objectifs-pedagogiques)
3. [Fonctionnalites principales](#fonctionnalites-principales)
4. [Architecture du projet](#architecture-du-projet)
5. [Diagramme UML (resume)](#diagramme-uml-resume)
6. [Demarrage d'une partie](#demarrage-dune-partie)
7. [Menu principal](#menu-principal)
8. [Systeme de combat](#systeme-de-combat)
9. [Systeme ACT et Mercy](#systeme-act-et-mercy)
10. [Categories de monstres](#categories-de-monstres)
11. [Items et inventaire](#items-et-inventaire)
12. [Chargement des fichiers CSV](#chargement-des-fichiers-csv)
13. [Conditions de fin de partie et fins multiples](#conditions-de-fin-de-partie-et-fins-multiples)
14. [Gestion de l'aleatoire](#gestion-de-laleatoire)
15. [Gestion des erreurs](#gestion-des-erreurs)
16. [Compilation et execution](#compilation-et-execution)
17. [Arborescence des fichiers](#arborescence-des-fichiers)
18. [Evaluation et bareme](#evaluation-et-bareme)
19. [Bonus possibles](#bonus-possibles)
20. [Auteurs](#auteurs)

---

## Presentation du projet

**ALTERDUNE** est un mini-jeu RPG en console developpe en **C++**, inspire de l'univers des RPG au tour par tour. Le joueur affronte des monstres dans des combats strategiques, avec la possibilite de les **tuer** ou de les **epargner** grace au systeme de **Mercy**. Le jeu propose des **fins multiples** determinees par les choix du joueur tout au long de la partie.

Le projet est realise en **binome** et met en oeuvre les concepts fondamentaux de la **Programmation Orientee Objet (POO)** : encapsulation, heritage, polymorphisme et composition.

---

## Objectifs pedagogiques

Ce projet a pour but de mettre en pratique les concepts suivants :

**Encapsulation** — Tous les attributs des classes sont prives. L'acces et la modification se font exclusivement via des accesseurs (getters) et modificateurs (setters) publics. Cela garantit l'integrite des donnees internes de chaque objet.

**Heritage** — Le joueur et les monstres partagent des caracteristiques communes (HP, nom, statistiques). Une classe de base abstraite (par ex. `Entity` ou `Character`) est heritee par les classes `Player` et `Monster`, evitant ainsi la duplication de code.

**Polymorphisme** — Le comportement des monstres varie en fonction de leur categorie (`NORMAL`, `MINIBOSS`, `BOSS`). Le nombre d'actions ACT disponibles (2, 3 ou 4) est determine dynamiquement par la categorie du monstre, ce qui illustre le polymorphisme.

**Composition** — Le joueur possede un inventaire (collection d'items), le jeu contient un bestiaire (collection de monstres vaincus), et chaque monstre est associe a des actions ACT. Ces relations « a un » illustrent la composition d'objets.

**Lecture de fichiers** — Les donnees du jeu (items, monstres) sont chargees dynamiquement depuis des fichiers CSV au demarrage, et non codees en dur dans le programme.

---

## Fonctionnalites principales

- Saisie du nom du personnage au lancement
- Chargement dynamique des items et monstres depuis des fichiers CSV
- Menu principal interactif avec 5 options (Bestiaire, Combat, Stats, Items, Quitter)
- Systeme de combat au tour par tour avec 4 actions (FIGHT, ACT, ITEM, MERCY)
- Catalogue de 8+ actions ACT predefinies avec textes humoristiques
- Systeme de jauge Mercy permettant d'epargner les monstres
- 3 categories de monstres avec polymorphisme (NORMAL, MINIBOSS, BOSS)
- Inventaire avec utilisation d'items en et hors combat
- Bestiaire enregistrant les monstres vaincus et le resultat (Tue / Epargne)
- 3 fins multiples : Genocidaire, Pacifiste, Neutre
- Gestion des erreurs de fichiers (introuvable, ligne mal formee)

---

## Architecture du projet

Le projet suit une architecture orientee objet. Les classes principales et leurs responsabilites sont decrites ci-dessous.

### Classe abstraite `Entity` (ou `Character`)

Classe de base pour le joueur et les monstres. Elle contient les attributs communs :
- `name` (string) — nom de l'entite
- `hp` (int) — points de vie actuels
- `hpMax` (int) — points de vie maximum
- `atk` (int) — statistique d'attaque
- `def` (int) — statistique de defense

Methodes : `takeDamage()`, `isAlive()`, `heal()`, getters/setters.

### Classe `Player` (herite de `Entity`)

Represente le personnage du joueur. Attributs supplementaires :
- `inventory` (vector d'`Item`) — inventaire du joueur
- `victories` (int) — nombre de combats gagnes (objectif : 10)
- `monstersKilled` (int) — nombre de monstres tues
- `monstersSpared` (int) — nombre de monstres epargnes

### Classe `Monster` (herite de `Entity`)

Represente un ennemi. Attributs supplementaires :
- `category` (enum : `NORMAL`, `MINIBOSS`, `BOSS`) — categorie du monstre
- `mercy` (int) — jauge de Mercy actuelle (commence a 0)
- `mercyGoal` (int) — seuil de Mercy a atteindre (en general 100)
- `actIDs` (vector de string) — identifiants des actions ACT disponibles

Le polymorphisme s'exprime ici : selon la categorie, 2, 3 ou 4 actions ACT sont disponibles.

### Classe `Item`

Represente un objet consommable :
- `name` (string) — nom de l'item
- `type` (string) — type de l'item (ex. `HEAL`)
- `value` (int) — valeur de l'effet (ex. HP restaures)
- `quantity` (int) — quantite disponible

### Structure `ACTAction`

Represente une action ACT dans le catalogue :
- `id` (string) — identifiant unique (ex. `JOKE`, `COMPLIMENT`)
- `text` (string) — texte drole/absurde affiche lors de l'execution
- `mercyImpact` (int) — variation de Mercy (positif, negatif, ou 0)

### Classe `Game` (ou `GameManager`)

Orchestre la logique du jeu : menu principal, lancement des combats, conditions de fin, chargement des fichiers, gestion du bestiaire.

### Classe `Combat`

Gere la boucle de combat : affichage du menu de combat, execution des actions du joueur, tour du monstre, verification des conditions de victoire/defaite.

### Classe `Bestiary`

Stocke les monstres vaincus avec le resultat du combat (tue ou epargne). Permet l'affichage formate du bestiaire.

---

## Diagramme UML (resume)

Le diagramme UML complet doit etre realise separement (outil recommande : draw.io, Lucidchart, PlantUML, ou Mermaid). Voici un apercu textuel des relations :

```
            +----------------+
            |    Entity      |  (abstraite)
            |----------------|
            | - name         |
            | - hp / hpMax   |
            | - atk / def    |
            |----------------|
            | + takeDamage() |
            | + isAlive()    |
            +-------+--------+
                    | heritage
           +--------+--------+
           |                  |
    +------+------+    +------+------+
    |   Player    |    |   Monster   |
    |-------------|    |-------------|
    | - inventory |    | - category  |
    | - victories |    | - mercy     |
    | - killed    |    | - mercyGoal |
    | - spared    |    | - actIDs    |
    +------+------+    +-------------+
           | composition
    +------+------+
    |    Item     |
    |-------------|
    | - name      |
    | - type      |
    | - value     |
    | - quantity  |
    +-------------+

    +----------------+
    |  ACTAction     |  (catalogue predefini)
    |----------------|
    | - id           |
    | - text         |
    | - mercyImpact  |
    +----------------+

    +----------------+       +--------------+
    |    Game        |<>-----|   Combat     |
    |----------------|       |--------------|
    | - player       |       | - player     |
    | - monsterPool  |       | - monster    |
    | - bestiary     |       | - actCatalog |
    | - actCatalog   |       +--------------+
    +----------------+
```

---

## Demarrage d'une partie

Au lancement du programme, la sequence suivante se deroule :

**Etape 1 — Saisie du nom.** Le joueur entre le nom de son personnage via la console.

**Etape 2 — Chargement des fichiers.** Le programme lit les fichiers `items.csv` et `monsters.csv` situes dans le repertoire du projet. En cas d'erreur (fichier introuvable ou ligne mal formee), le programme affiche un message d'erreur explicite et se termine proprement.

**Etape 3 — Affichage du resume.** Le jeu affiche un recapitulatif : le nom du joueur, ses HP (actuels / max), et la liste de ses items avec leurs quantites.

**Etape 4 — Menu principal.** Le joueur accede au menu principal et peut commencer a interagir avec le jeu.

---

## Menu principal

Le menu principal est le point central du jeu. Il est affiche en boucle tant que la partie n'est pas terminee (< 10 victoires et joueur vivant). Les options sont les suivantes.

### 1. Bestiaire

Affiche la liste de tous les monstres vaincus jusqu'a present. Pour chaque monstre :
- Nom du monstre
- Categorie (NORMAL / MINIBOSS / BOSS)
- Statistiques (HP max, ATK, DEF)
- Resultat du combat : **Tue** ou **Epargne**

Si aucun monstre n'a ete vaincu, un message approprie est affiche.

### 2. Demarrer un combat

Lance un combat contre un monstre tire **aleatoirement** depuis la liste chargee depuis `monsters.csv`. Le meme monstre peut etre tire plusieurs fois (les donnees sont reinitialisees a chaque combat : HP pleins, Mercy a 0).

### 3. Statistiques du personnage

Affiche les informations completes sur le joueur :
- Nom du joueur
- HP actuel / HP maximum
- Nombre de monstres tues
- Nombre de monstres epargnes
- Nombre de victoires (sur 10)

### 4. Items

Affiche l'inventaire complet du joueur avec pour chaque item :
- Nom de l'item
- Quantite restante
- Description de l'effet (ex. : « Soigne 15 HP »)

Le joueur peut **utiliser un item depuis ce menu** (hors combat). L'utilisation reduit la quantite de 1 et applique l'effet (soin, etc.). Les items a quantite 0 ne peuvent plus etre utilises.

### 5. Quitter

Met fin au programme proprement.

---

## Systeme de combat

Le combat est au **tour par tour** : le joueur agit en premier, puis le monstre riposte (s'il est encore vivant).

### Deroulement d'un tour

1. **Affichage de l'etat** — HP du joueur, HP du monstre, jauge Mercy du monstre.
2. **Menu de combat** — Le joueur choisit parmi : `FIGHT` | `ACT` | `ITEM` | `MERCY`
3. **Action du joueur** — L'action choisie est executee.
4. **Verification** — Si le monstre est vaincu (HP <= 0 ou Mercy epargne), le combat se termine.
5. **Tour du monstre** — Le monstre attaque le joueur.
6. **Verification** — Si le joueur tombe a 0 HP, la partie est immediatement perdue (Game Over).

### FIGHT

Le joueur attaque le monstre. Les degats infliges sont calcules aleatoirement (voir section Aleatoire). Si les HP du monstre tombent a 0, il est **tue**. Le compteur de victoires augmente de 1 et le monstre est ajoute au bestiaire avec le statut « Tue ».

### ACT

Le joueur choisit une action parmi celles disponibles pour le monstre courant (2 pour NORMAL, 3 pour MINIBOSS, 4 pour BOSS). L'action est recherchee dans le **catalogue d'actions ACT** predefini dans le code. Lorsqu'une action est executee :
- Le **texte drole/absurde** associe est affiche en console.
- La **jauge Mercy** est modifiee selon l'impact defini (positif = apaisement, negatif = enervement).
- La Mercy reste **bornee** entre 0 et le `mercyGoal` du monstre (en general 100).

### ITEM

Le joueur utilise un item de son inventaire. Par exemple, une Potion qui restaure des HP. L'utilisation consomme le tour (le monstre attaquera ensuite). Si l'inventaire est vide ou que l'item choisi a une quantite de 0, le joueur doit choisir une autre action.

### MERCY

Disponible uniquement si la jauge Mercy du monstre a atteint ou depasse le `mercyGoal` (>= 100). Le monstre est **epargne** : le combat est gagne, le compteur de victoires augmente de 1, et le monstre est ajoute au bestiaire avec le statut « Epargne ». Si la Mercy est insuffisante, l'action echoue et un message le signale.

---

## Systeme ACT et Mercy

### Catalogue d'actions ACT

Les actions ACT sont **predefinies dans le code C++** (pas dans les fichiers CSV). Elles sont stockees dans une structure de donnees (par ex. `std::map<std::string, ACTAction>` ou `std::vector<ACTAction>`).

Chaque action possede :
- Un **identifiant** unique (ex. `JOKE`, `COMPLIMENT`, `INSULT`, `DANCE`, etc.)
- Un **texte** affiche lorsque l'action est executee (texte drole ou absurde)
- Un **impact sur Mercy** : entier positif (apaise), negatif (enerve), ou 0 (neutre)

**Contraintes du catalogue :**
- Minimum **8 actions** differentes dans le catalogue.
- Au moins **2 actions avec un impact negatif** (qui enervent le monstre).
- La Mercy est toujours **bornee** : elle ne descend jamais sous 0 et ne depasse jamais le `mercyGoal`.

### Exemple de catalogue

| ID             | Texte affiche                                        | Impact Mercy |
|----------------|------------------------------------------------------|:------------:|
| `COMPLIMENT`   | « Tu as de tres beaux yeux... pour un monstre. »     |     +15      |
| `JOKE`         | « Pourquoi les monstres ne font pas de maths ? Parce qu'ils ont peur des problemes ! » | +10 |
| `DISCUSS`      | « Vous discutez du beau temps... il pleut des epees. » |    +5       |
| `DANCE`        | « Vous faites la Macarena. Le monstre est perplexe. » |    +20      |
| `PET`          | « Vous caressez le monstre. Il ronronne... bizarrement. » | +15     |
| `OBSERVE`      | « Vous observez le monstre attentivement. Il rougit. » |   +10      |
| `OFFER_SNACK`  | « Vous offrez un snack. Le monstre l'engloutit d'un coup. » | +10   |
| `REASON`       | « Vous tentez de raisonner le monstre. Il semble reflechir. » | +20 |
| `INSULT`       | « Vous insultez le monstre. Il est furieux ! »        |     -20      |
| `THREATEN`     | « Vous menacez le monstre. L'ambiance est glaciale. » |    -15      |

Le fichier `monsters.csv` ne contient que les identifiants (ex. `JOKE`, `INSULT`). Le texte et l'impact sont definis dans le code.

---

## Categories de monstres

Les monstres sont repartis en 3 categories qui determinent leur puissance et le nombre d'actions ACT auxquelles le joueur a acces :

| Categorie  | Nb actions ACT | Description                          |
|------------|:--------------:|--------------------------------------|
| `NORMAL`   |       2        | Monstre standard, 2 actions ACT     |
| `MINIBOSS` |       3        | Monstre intermediaire, 3 actions ACT |
| `BOSS`     |       4        | Monstre puissant, 4 actions ACT     |

Dans le fichier `monsters.csv`, les champs `act3` et `act4` valent `-` si non utilises (ex. un monstre NORMAL n'a que `act1` et `act2`).

---

## Items et inventaire

Les items sont charges depuis `items.csv` au demarrage. Ils sont stockes dans l'inventaire du joueur.

### Format de `items.csv`

```
nom;type;valeur;quantite
```

- **nom** — Nom affiche de l'item (ex. `Potion`)
- **type** — Type de l'item. Pour simplifier, seul le type `HEAL` (soin) est requis.
- **valeur** — Nombre de HP restaures lors de l'utilisation.
- **quantite** — Nombre d'exemplaires disponibles au debut de la partie.

### Exemple de fichier `items.csv`

```csv
Potion;HEAL;15;3
Snack;HEAL;8;5
SuperPotion;HEAL;30;1
```

### Regles d'utilisation

- Un item peut etre utilise **en combat** (action ITEM) ou **depuis le menu principal** (option Items).
- Chaque utilisation **consomme 1 unite** de l'item.
- Un item a **quantite 0** ne peut plus etre utilise.
- L'effet `HEAL` restaure les HP du joueur **sans depasser le HP max**.

---

## Chargement des fichiers CSV

Le jeu charge obligatoirement **deux fichiers CSV** au demarrage.

### `items.csv`

Separateur : `;`  
Format : `nom;type;valeur;quantite`

### `monsters.csv`

Separateur : `;`  
Format : `categorie;nom;hp;atk;def;mercyGoal;act1;act2;act3;act4`

**Regles pour `monsters.csv` :**
- Un monstre `NORMAL` utilise `act1` et `act2` uniquement. Les champs `act3` et `act4` valent `-`.
- Un monstre `MINIBOSS` utilise `act1`, `act2`, `act3`. Le champ `act4` vaut `-`.
- Un monstre `BOSS` utilise les 4 champs `act1` a `act4`.
- Les identifiants d'actions doivent correspondre a des entrees existantes dans le catalogue ACT code en dur.

### Exemple de fichier `monsters.csv`

```csv
NORMAL;Froggit;30;7;1;100;COMPLIMENT;DISCUSS;-;-
MINIBOSS;MimicBox;45;10;2;100;OBSERVE;PET;OFFER_SNACK;-
BOSS;QueenByte;80;15;4;100;REASON;DANCE;JOKE;INSULT
```

---

## Conditions de fin de partie et fins multiples

### Condition de victoire

La partie se termine lorsque le joueur atteint **10 victoires** (10 combats gagnes, que le monstre ait ete tue ou epargne).

### Condition de defaite

Si les HP du joueur tombent a **0** pendant un combat, la partie est immediatement perdue (**Game Over**).

### Fins multiples

A la fin de la partie (10 victoires), le jeu affiche une fin narrative differente selon l'historique du joueur :

| Fin                 | Condition                                            |
|---------------------|------------------------------------------------------|
| **Fin Genocidaire** | Le joueur a tue **tous** les monstres (0 epargne)    |
| **Fin Pacifiste**   | Le joueur a epargne **tous** les monstres (0 tue)    |
| **Fin Neutre**      | Le joueur a a la fois tue et epargne des monstres    |

---

## Gestion de l'aleatoire

### Formule de degats

A chaque attaque (du joueur ou du monstre), les degats sont calcules ainsi :

```
degats = random(0, HPmax_defenseur)
```

`HPmax_defenseur` correspond au HP maximum de la cible (monstre si le joueur attaque, joueur si le monstre attaque).

**Regles :**
- Si `degats == 0` : coup rate, aucun degat inflige.
- Sinon : les HP du defenseur diminuent de la valeur des degats.
- Les HP ne descendent **jamais** sous 0.
- Si les HP atteignent 0, l'entite est vaincue.

### Implementation recommandee

Utiliser la bibliotheque `<random>` de C++ (et non `rand()`) pour une meilleure qualite d'aleatoire :

```cpp
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

int rollDamage(int hpMaxDefender) {
    std::uniform_int_distribution<int> dist(0, hpMaxDefender);
    return dist(gen);
}
```

### Tirage aleatoire de monstres

Au debut de chaque combat, un monstre est tire aleatoirement depuis le pool charge depuis `monsters.csv`. Un meme monstre peut apparaitre plusieurs fois au cours de la partie (les stats sont reinitialisees a chaque apparition).

---

## Gestion des erreurs

Le programme doit gerer au minimum les erreurs suivantes :

**Fichier introuvable** — Si `items.csv` ou `monsters.csv` n'est pas trouve, le programme affiche un message d'erreur clair (ex. : `Erreur : fichier items.csv introuvable.`) et se termine proprement.

**Ligne mal formee** — Si une ligne du CSV ne respecte pas le format attendu (nombre de champs incorrect, type invalide, valeur non numerique, etc.), le programme peut soit ignorer la ligne (avec avertissement), soit arreter avec un message d'erreur. Le choix est libre, mais doit etre coherent.

**Saisies invalides** — Les entrees utilisateur dans les menus doivent etre verifiees. Un choix hors limites affiche un message et redemande la saisie.

**Identifiant ACT inconnu** — Si un identifiant d'action dans `monsters.csv` ne correspond a aucune entree du catalogue, l'erreur doit etre signalee.

---

## Compilation et execution

### Prerequis

- Compilateur C++ compatible **C++11** minimum (C++17 recommande)
- `g++`, `clang++`, ou MSVC

### Compilation (exemple avec g++)

```bash
g++ -std=c++17 -Wall -Wextra -o alterdune main.cpp Game.cpp Player.cpp Monster.cpp Item.cpp Combat.cpp Bestiary.cpp
```

### Execution

```bash
./alterdune
```

Les fichiers `items.csv` et `monsters.csv` doivent etre dans le **meme repertoire** que l'executable (ou le chemin doit etre adapte dans le code).

### Compilation avec Makefile (recommande)

Un `Makefile` peut etre fourni pour simplifier la compilation :

```bash
make        # compile le projet
make clean  # supprime les fichiers objets et l'executable
./alterdune # lance le jeu
```

---

## Arborescence des fichiers

```
ALTERDUNE/
├── README.md               # Ce fichier
├── Makefile                 # (optionnel) Automatisation de la compilation
├── items.csv                # Fichier de donnees : items du joueur
├── monsters.csv             # Fichier de donnees : monstres ennemis
├── UML/
│   └── diagramme_uml.png   # Diagramme UML du projet
├── include/                 # Fichiers d'en-tete (.h)
│   ├── Entity.h
│   ├── Player.h
│   ├── Monster.h
│   ├── Item.h
│   ├── ACTAction.h
│   ├── Combat.h
│   ├── Bestiary.h
│   └── Game.h
├── src/                     # Fichiers source (.cpp)
│   ├── main.cpp
│   ├── Entity.cpp
│   ├── Player.cpp
│   ├── Monster.cpp
│   ├── Item.cpp
│   ├── Combat.cpp
│   ├── Bestiary.cpp
│   └── Game.cpp
└── docs/                    # (optionnel) Documentation supplementaire
    └── sujet_projet.pdf
```

---

## Evaluation et bareme

### TD11-TD12 : Mini-suivi (20% de la note finale)

| Critere                    | Points |
|----------------------------|:------:|
| UML initial                |   8    |
| Avancees techniques        |   8    |
| Clarte des explications    |   4    |
| **Total mini-suivi**       | **20** |

### TD15-TD16 : Soutenance — Partie 1 (80% de la note finale)

| Critere                                                    | Points |
|------------------------------------------------------------|:------:|
| Diagramme UML final                                        |   2    |
| Heritage (structuration joueur/monstres/categories)        |   2    |
| Classe abstraite                                           |   1    |
| Classes derivees                                           |   1    |
| Encapsulation (attributs prives + getters/setters)         |   2    |
| Polymorphisme (2/3/4 actions ACT selon categorie)          |   2    |
| Systeme ACT et Mercy (catalogue + effets + bornage)        |   2    |
| Lecture des fichiers CSV (parsing + gestion d'erreurs)     |   2    |
| Menu principal (bestiaire, stats, items, combat)           |   3    |
| Systeme de combat (FIGHT/ACT/ITEM/MERCY + tour monstre)   |   3    |
| Fin a 10 victoires + fins multiples                        |   2    |
| Presentation (clarte, demo, explications techniques)       |   2    |
| **Total soutenance Partie 1**                              | **22** |

### TD17-TD18 : Soutenance — Partie 2 (Questions/Reponses)

Session orale individuelle avec l'enseignant. Evalue la **maitrise du code** et des concepts POO par chaque membre du binome. Une mauvaise maitrise peut entrainer une **penalite importante** sur la note finale.

### Formule de note finale

```
Note finale = 0.2 * Note_mini_suivi + 0.8 * Note_soutenance_P1 + bonus - penalite
```

---

## Bonus possibles

Un bonus de **jusqu'a +4 points** peut etre accorde si le projet est juge « sur-salient » (au-dela des attentes, sans complexification inutile). Idees de bonus :

- Ajout de types d'items supplementaires (buff d'attaque, bouclier temporaire, etc.)
- Animations ASCII en console
- Systeme de sauvegarde/chargement de partie
- Effets sonores (bibliotheque audio console)
- IA de monstre amelioree (comportement adaptatif)
- Interface avec couleurs ANSI
- Systeme de niveaux / experience pour le joueur
- Evenements aleatoires entre les combats
- Items rares drop par les monstres
- Mode « difficile » avec des stats de monstres augmentees

---

## Auteurs

| Nom           | Role        |
|---------------|-------------|
| [Nom 1]       | Developpeur |
| [Nom 2]       | Developpeur |

**ESILV — Programmation Orientee Objet en C++ — 2025-2026**
