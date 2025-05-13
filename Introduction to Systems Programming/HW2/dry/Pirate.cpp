
#include "Pirate.h"


Pirate::Pirate(const string &name, int bounty, DEVIL_FRUIT devilFruit) : name(
        name),
                                                                         bounty(bounty),
                                                                         devilFruit(
                                                                                 devilFruit) {}


Pirate::Pirate(const string &name) : name(name) {}

void Pirate::setName(const string &name) {
    this->name = name;
}


void Pirate::setDevilFruit(DEVIL_FRUIT devilFruit) {
    this->devilFruit = devilFruit;
}


std::string Pirate::getName() const {
    return name;
}


void Pirate::setBounty(int bounty) {
    this->bounty = bounty;

}

int Pirate::getBounty() const {
    return bounty;
}

DEVIL_FRUIT Pirate::getDevilFruit() const {
    return devilFruit;
}


const char *const devilFruitNames[] = {
        "Gum Gum",
        "Smoke Smoke",
        "Flame Flame",
        "Rumble Rumble",
        "String String",
        "Ice Ice",
        "None"
};

std::ostream &operator<<(std::ostream &os, const Pirate &pirate) {
    os << pirate.name << " (Bounty: " << pirate.bounty
       << ", Devil Fruit: " << devilFruitNames[pirate.devilFruit] << ")";
    return os;
}
