# ALTERDUNE — Contexte et suivi d'implementation

## Etat actuel
- Toutes les phases implementees (0 a 7)

## Decisions prises
- Polymorphisme : sous-classes MonsterNormal/MiniBoss/Boss avec unique_ptr<Monster> et virtual clone()
- L'enum MonsterCategory et le switch/case de base dans Monster.cpp sont conserves comme fallback
- Formule degats : baseDamage + ATK_attaquant - DEF_defenseur (min 1 si pas rate, 0 si rate)
- Buffs ATK/DEF : temporaires par combat (resetBuffs apres chaque combat)
- Buffs d'evenements : persistent jusqu'au prochain combat (reset au meme endroit)
- Evenements aleatoires : 40% de chance apres chaque victoire, 8 evenements
- Couleurs ANSI : appliquees dans Combat.cpp, Game.cpp, Player.cpp, Bestiary.cpp, Item.cpp, MonsterXxx.cpp
- UML en format PlantUML (diagramme.puml) — a rendre en PNG via outil en ligne
- README.md cree avec placeholders [Nom 1] et [Nom 2] pour les auteurs

## Style de code
- K&R braces, 4 espaces, camelCase methodes, PascalCase classes
- `using namespace std;` partout
- Commentaires en francais avec //
- Pas d'espace entre mot-cle et ( : if(cond), for(...), while(...)
- Parametres par valeur, const sur getters
- Boucles : for(int i=0; i < (int)vec.size(); i++)

## Fichiers crees
- context.md
- include/Colors.h
- include/MonsterNormal.h, src/MonsterNormal.cpp
- include/MonsterMiniBoss.h, src/MonsterMiniBoss.cpp
- include/MonsterBoss.h, src/MonsterBoss.cpp
- UML/diagramme.puml
- README.md

## Fichiers modifies
- include/Monster.h (virtual getNbActs, getCategoryStr, clone)
- src/Monster.cpp (ajout clone)
- include/Game.h (unique_ptr monsterPool, struct RandomEvent, eventCatalog, triggerRandomEvent)
- src/Game.cpp (factory pattern, events, resetBuffs, couleurs)
- include/Combat.h (unique_ptr<Monster>, rollDamage signature)
- src/Combat.cpp (monster->, formule degats, couleurs)
- include/Player.h (atkBuff, defBuff, getters/setters/resetBuffs)
- src/Player.cpp (init buffs, useItem ATK_BUFF/DEF_BUFF, couleurs)
- src/Item.cpp (displayItem ATK_BUFF/DEF_BUFF, couleurs)
- items.csv (ajout AtkBoost, DefBoost)
- Makefile (ajout 3 nouvelles sources)
- src/Bestiary.cpp (couleurs)

## A faire par l'utilisateur
- Renseigner les noms des auteurs dans README.md
- Generer le PNG du diagramme UML via plantuml ou plantuml.com
- Compiler avec `make clean && make` et tester
