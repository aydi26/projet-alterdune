#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include <vector>
#include <string>
#include <memory>

// catégories de monstres, utilisées pour la factory et le polymorphisme
enum MonsterCategory { NORMAL, MINIBOSS, BOSS };

class Monster : public Entity {
private:
    MonsterCategory category;
    int mercy;          // jauge mercy courante, initialisée à 0 en début de combat
    int mercyGoal;      // seuil à atteindre pour pouvoir épargner le monstre
    vector<string> actIDs;  // identifiants des actions ACT disponibles pour ce monstre

public:
    Monster(string name,int hp, int atk, int def, MonsterCategory cat,int mercyGoal,vector<string> acts);
    Monster();

    // ajoute (ou retire si négatif) de la mercy, en clampant le résultat entre 0 et mercyGoal
    void addMercy(int amount);

    // true si la jauge mercy est suffisante pour épargner le monstre
    bool canBeSpared() const;

    // déclarée virtuelle pour permettre la spécialisation dans les sous-classes
    virtual int getNbActs() const;

    // restaure les hp au maximum et remet la mercy à 0 avant un nouveau combat
    void resetForCombat();

    // getters
    MonsterCategory getCategory() const;
    virtual string getCategoryStr() const;
    int getMercy() const;
    int getMercyGoal() const;
    vector<string> getActIDs() const;

    void displayInfo() const override;

    // permet le clonage polymorphe via un pointeur sur la classe de base
    virtual unique_ptr<Monster> clone() const;
};

#endif
