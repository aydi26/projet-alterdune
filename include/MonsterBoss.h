#ifndef MONSTERBOSS_H
#define MONSTERBOSS_H

#include "Monster.h"
#include <memory>

// monstre de catégorie boss, dispose de 4 actions ACT et de stats généralement plus élevées
class MonsterBoss : public Monster {
public:
    MonsterBoss(string name,int hp, int atk,int def, int mercyGoal, vector<string> acts);

    int getNbActs() const override;
    string getCategoryStr() const override;
    void displayInfo() const override;
    unique_ptr<Monster> clone() const override;
};

#endif
