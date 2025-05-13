
#pragma once

#include <iostream>

using std::string;

enum DEVIL_FRUIT {
    GUM_GUM_FRUIT,
    SMOKE_SMOKE_FRUIT,
    FLAME_FLAME_FRUIT,
    RUMBLE_RUMBLE_FRUIT,
    STRING_STRING_FRUIT,
    ICE_ICE_FRUIT,
    NONE
};

class Pirate {
private:
    string name;
    int bounty;

public:
    DEVIL_FRUIT devilFruit;

    Pirate(const string &name, int bounty,
           DEVIL_FRUIT devilFruit = DEVIL_FRUIT::NONE);

    Pirate() = default;

    Pirate(const string &name);


    ~Pirate() = default;

    void setName(const string &name);


    void setDevilFruit(DEVIL_FRUIT devilFruit);

    string getName() const;

    DEVIL_FRUIT getDevilFruit() const;

    void setBounty(int bounty);

    int getBounty() const;


    friend std::ostream &operator<<(std::ostream &os, const Pirate &pirate);
};
