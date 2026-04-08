#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include <vector>
#include <string>
#include <memory>

// enum pour les categories de monstres
enum MonsterCategory { NORMAL, MINIBOSS, BOSS };

class Monster : public Entity {
private:
    MonsterCategory category;
    int mercy;      // jauge mercy actuelle (commence a 0)
    int mercyGoal;  // seuil a atteindre pour epargner
    vector<string> actIDs; // les actions ACT disponibles

public:
    // constructeur
    Monster(string name, int hp, int atk, int def, MonsterCategory cat, int mercyGoal, vector<string> acts);
    // constructeur par defaut
    Monster();

    // ajouter de la mercy (peut etre negatif)
    void addMercy(int amount);

    // verifier si le monstre peut etre epargne
    bool canBeSpared() const;

    // nombre d'actions ACT selon la categorie (virtuel pour le polymorphisme)
    virtual int getNbActs() const;

    // remettre le monstre a zero pour un nouveau combat
    void resetForCombat();

    // getters
    MonsterCategory getCategory() const;
    virtual string getCategoryStr() const;
    int getMercy() const;
    int getMercyGoal() const;
    vector<string> getActIDs() const;

    // affichage (override de la methode abstraite)
    void displayInfo() const override;

    // cloner le monstre (pour le polymorphisme avec unique_ptr)
    virtual unique_ptr<Monster> clone() const;
};

#endif
