#ifndef MONSTERNORMAL_H
#define MONSTERNORMAL_H

#include "Monster.h"
#include <memory>

// monstre standard, dispose de 2 actions ACT
class MonsterNormal : public Monster {
public:
    MonsterNormal(string name,int hp,int atk, int def, int mercyGoal, vector<string> acts);

    // overrides nécessaires pour le polymorphisme
    int getNbActs() const override;
    string getCategoryStr() const override;
    void displayInfo() const override;
    unique_ptr<Monster> clone() const override;
};

#endif
