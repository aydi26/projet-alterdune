#ifndef MONSTERBOSS_H
#define MONSTERBOSS_H

#include "Monster.h"
#include <memory>

class MonsterBoss : public Monster {
public:
    // constructeur
    MonsterBoss(string name, int hp, int atk, int def, int mercyGoal, vector<string> acts);

    // overrides pour le polymorphisme
    int getNbActs() const override;
    string getCategoryStr() const override;
    void displayInfo() const override;
    unique_ptr<Monster> clone() const override;
};

#endif
