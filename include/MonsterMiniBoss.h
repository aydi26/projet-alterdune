#ifndef MONSTERMINIBOSS_H
#define MONSTERMINIBOSS_H

#include "Monster.h"
#include <memory>

// monstre intermédiaire, dispose de 3 actions ACT
class MonsterMiniBoss : public Monster {
public:
    MonsterMiniBoss(string name, int hp,int atk,int def,int mercyGoal,vector<string> acts);

    int getNbActs() const override;
    string getCategoryStr() const override;
    void displayInfo() const override;
    unique_ptr<Monster> clone() const override;
};

#endif
