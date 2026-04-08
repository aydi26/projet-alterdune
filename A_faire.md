# A faire — Tests ALTERDUNE avant rendu

Checklist a executer dans l'ordre. Cocher au fur et a mesure.

## Pre-requis

- [ ] Terminal ANSI ouvert (Windows Terminal ou Git Bash, **pas** `cmd.exe` legacy)
- [ ] g++ avec C++17 disponible (`g++ --version`)
- [ ] `make` disponible (sinon `mingw32-make`)
- [ ] Working directory = `C:\Users\adria\projet-alterdune\` (pour que les CSV soient trouves)

## Avant rendu (administratif)

- [ ] Remplir les `[Nom 1]` et `[Nom 2]` dans `README.md`
- [ ] Generer le PNG du diagramme : `plantuml UML/diagramme.puml` ou via [plantuml.com](https://www.plantuml.com/plantuml/uml)
- [ ] Verifier que `items.csv` et `monsters.csv` sont bien presents a la racine

## Etape 1 — Compilation

```bash
make clean && make
```

- [ ] Aucune erreur
- [ ] Noter les warnings eventuels (pour pouvoir les justifier)
- [ ] `alterdune` (ou `alterdune.exe`) genere

## Etape 2 — Lancement et chargement

```bash
./alterdune
```

- [ ] Banniere "BIENVENUE DANS ALTERDUNE" en cyan
- [ ] Saisie du nom acceptee
- [ ] Recap : HP 100/100, 5 items affiches, "7 monstre(s) charge(s)"
- [ ] Menu principal avec 5 options

**Test negatif :**
- [ ] Renommer `items.csv` → relancer → message d'erreur propre, pas de crash. Remettre.
- [ ] Idem avec `monsters.csv`

## Etape 3 — Menu (avant tout combat)

- [ ] Option **1 Bestiaire** → "Aucun monstre vaincu pour le moment"
- [ ] Option **3 Statistiques** → HP 100/100, ATK 10, DEF 5, 0/10 victoires, **pas** de "Buffs actifs"
- [ ] Option **4 Items** → 5 items listes, refuser l'utilisation
- [ ] Entrees invalides (`abc`, `0`, `99`) → reboucle sans crash

## Etape 4 — Polymorphisme (CRITIQUE)

Lancer 5+ combats jusqu'a voir chaque categorie :

- [ ] NORMAL (Froggit/Slimey/Batty) affiche **2 actions** dans ACT
- [ ] MINIBOSS (MimicBox/ShadowWolf) affiche **3 actions**
- [ ] BOSS (QueenByte/DarkLord) affiche **4 actions**

Si tout le monde affiche le meme nombre → bug `clone()` ou factory dans `Game::loadMonsters`.

## Etape 5 — 4 actions de combat

Dans un combat :
- [ ] **FIGHT** → degats infliges OU coup rate, HP du monstre baisse
- [ ] **ACT** → texte affiche, mercy +X visible, jauge progresse
- [ ] **ITEM** → Potion soigne, AtkBoost augmente les degats du FIGHT suivant, **`0` annule sans consommer le tour**
- [ ] **MERCY** echoue (jauge vide) → message + **le monstre n'attaque pas** (HP joueur inchanges)

## Etape 6 — Mercy et epargne

- [ ] Combat Froggit : 7× COMPLIMENT (15×7=105) → MERCY fonctionne
- [ ] Statistiques : `victories=1`, `monstersSpared=1`
- [ ] Bestiaire : entree avec `Resultat : Epargne` en vert
- [ ] Sur un BOSS, INSULT/THREATEN font baisser la jauge (sans descendre sous 0)

## Etape 7 — Items et buffs

- [ ] AtkBoost en combat → degats plus eleves
- [ ] **Apres** le combat → Statistiques **ne montre plus** "Buffs actifs"
- [ ] Idem DefBoost
- [ ] SuperPotion (+30) hors combat → HP remonte, plafonne a 100
- [ ] Vider une stack (3× Potion) → "Cet item n'est plus disponible"

## Etape 8 — Evenements aleatoires

Apres ~10 combats :
- [ ] Au moins un **HEAL** (Source miraculeuse / Meditation) restaure des HP
- [ ] Au moins un **DAMAGE** (Piege a ours / Embuscade) retire des HP
- [ ] Cas critique : si HP bas + DAMAGE → "Vous avez succombe a l'evenement... GAME OVER"
- [ ] Au moins un **ATK_BOOST** ou **DEF_BOOST** applique le buff
- [ ] Buff d'evenement persiste jusqu'au prochain combat, puis disparait
- [ ] Au moins un **ITEM_FIND** ajoute un Snack a l'inventaire

## Etape 9 — Les 3 fins (le plus long)

### Run 1 — FIN GENOCIDAIRE
- [ ] Strategie : FIGHT only, jamais MERCY
- [ ] Utiliser Potions/SuperPotion librement pour survivre
- [ ] 10 victoires → bloc rouge **=== FIN GENOCIDAIRE ===**
- [ ] `monstersSpared=0`, `monstersKilled=10`

### Run 2 — FIN PACIFISTE (le plus dur)
- [ ] Strategie : ACT only, MERCY des que possible, jamais FIGHT
- [ ] Utiliser tous les HEAL disponibles
- [ ] **Eviter INSULT et THREATEN** (mercy negatif)
- [ ] 10 victoires → bloc vert **=== FIN PACIFISTE ===**
- [ ] `monstersKilled=0`, `monstersSpared=10`
- [ ] Si infaisable apres 2-3 essais → noter pour la soutenance comme "balance a ajuster"

### Run 3 — FIN NEUTRE
- [ ] Strategie : alterner kill/spare
- [ ] 10 victoires → bloc jaune **=== FIN NEUTRE ===**
- [ ] Affichage correct "X tue(s) et Y epargne(s)" avec X+Y=10

## Etape 10 — Robustesse

- [ ] Se laisser tuer en combat → "GAME OVER" rouge, pas de crash
- [ ] Menu 5 (Quitter) → "Au revoir !" et sortie propre
- [ ] Entrees malformees (lettres, hors range) en combat → reboucle sans crash
- [ ] Ligne CSV corrompue ajoutee dans `items.csv` → "Attention : ligne mal formee" + jeu continue. **Remettre le fichier propre apres.**

## Recap commande par commande

```bash
cd C:\Users\adria\projet-alterdune

# 1. Build
make clean && make

# 2. Smoke test
./alterdune

# 3-8. Combats varies pour couvrir polymorphisme, items, buffs, events

# 9. 3 runs jusqu'a 10 victoires (genocide / pacifiste / neutre)

# 10. Tests negatifs (CSV manquant, mort, entrees invalides)
```

## Points a connaitre pour la soutenance

- **`unique_ptr<Monster>` + `clone()`** : ownership clair, polymorphisme sans fuite
- **Bornes mercy** (0 ↔ mercyGoal) : evite les valeurs aberrantes apres INSULT
- **Formule degats** scale avec hpMax cible : permet de tuer un BOSS avec atk=10
- **Buffs items = 1 combat**, **buffs events = jusqu'au prochain combat** : choix de design assume
- **Pas de tests automatises** : projet scolaire, validation manuelle

## Limitations connues a mentionner si questionne

- Balance pacifiste tres severe (degats moyens monstres ~50/tour)
- Couleurs ANSI invisibles sous `cmd.exe` legacy
- Pas de gestion explicite de l'EOF (`Ctrl+Z`)
