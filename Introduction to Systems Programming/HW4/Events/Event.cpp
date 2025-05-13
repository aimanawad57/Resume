#include "Event.h"
#include "Utilities.h"

using std::cout;
using std::endl;
using std::to_string;
using std::string;

/** Main Event Class Implementation **/

Event::Event(const string &description) : description(description) {}

string Event::getEventName() const {
    return description;
}


/** Rare Events Implementations **/

RareEvent::RareEvent(const string &description) : Event(description) {}

string RareEvent::getDescription() const {
    return description;
}


SolarEclipse::SolarEclipse() : RareEvent("SolarEclipse") {}

void SolarEclipse::make_move(Player &player) {
    int effect;
    if (player.getIsPlayerMagician() == IsMagician) {
        player.setForce(player.getForce() + 1);
        effect = 1;
    }
    else {
        if (player.getForce() == 0) {
            effect = 0;
        }
        else {
            player.setForce(player.getForce() - 1);
            player.updateCombatPower();
            effect = -1;
        }
    }
    printTurnOutcome(getSolarEclipseMessage(player, effect));
}

PotionsMerchant::PotionsMerchant() : RareEvent("PotionsMerchant") {}


void PotionsMerchant::make_move(Player &player) {
    int numBags = 0;
    if (player.getCoins() < 5 ||
        player.getHealthPoints() == player.getMaxHealthPoints() ||
        (player.getCharacter() == "RiskTaking" &&
         player.getHealthPoints() >= 50)) {
        printTurnOutcome(getPotionsPurchaseMessage(player, numBags));
        return; // doesn't have enough money or at max health
    }
    if (player.getCharacter() == "RiskTaking") {
        player.setHealthPoints(player.getHealthPoints() + 10);
        player.setCoins(player.getCoins() - 5);
        numBags = 1;
    }

    if (player.getCharacter() == "Responsible") { /** maybe do enum **/
        int tmpCurrentHP = player.getHealthPoints(), tmpCoins = player.getCoins();
        while (tmpCurrentHP < player.getMaxHealthPoints() && tmpCoins > 4) {
            tmpCurrentHP += 10;
            tmpCoins -= 5;
            numBags++;
        }
        if (tmpCurrentHP > player.getMaxHealthPoints()) {
            tmpCurrentHP = player.getMaxHealthPoints();
        }
        player.setHealthPoints(tmpCurrentHP);
        player.setCoins(tmpCoins);
    }
    printTurnOutcome(getPotionsPurchaseMessage(player, numBags));
}


/** Encounters Implementations **/

string Encounter::getDescription() const {
    return description + " (power " + to_string(combatPower) + ", loot " +
           to_string(loot) + ", damage "
           + to_string(damage) + ")";
}

Encounter::Encounter(const string &description, unsigned int combatPower,
                     unsigned int loot, unsigned int damage) : Event(
        description),
                                                               combatPower(
                                                                       combatPower),
                                                               loot(loot),
                                                               damage(damage) {}

/** the overall make_move function for general encounter handling **/

void Encounter::make_move(Player &player) {
    player.battle(*this);
}

Snail::Snail() : Encounter("Snail", 5, 2, 10) {}

Slime::Slime() : Encounter("Slime", 12, 5, 25) {}

int Encounter::getCombatPower() const {
    return (int)combatPower;
}

int Encounter::getDamage() const {
    return (int)damage;
}

int Encounter::getLoot() const {
    return (int)loot;
}

Balrog::Balrog() : Encounter("Balrog", 15, 100, 9001) {}

/* Balrog has special make_move cause it enhance his force by 2 every fight */

void Balrog::make_move(Player &player) {
    Encounter::make_move(player);
    this->combatPower += 2;
}

/* Implementations for Pack */


Pack::Pack() : Encounter("Pack", 0, 0, 0), size(0) {}

void Pack::addEvent(unique_ptr<Encounter> event) {
    combatPower += event->getCombatPower();
    loot += event->getLoot();
    damage += event->getDamage();
    pack.push_back(std::move(event));
    size++;
}

int Pack::countBalrogsCount() {
    int counter = 0;
    for (auto &encounter: pack) {
        if (encounter->getEventName() == "Balrog") {
            counter++;
        }
        else {
            if (encounter->getEventName() == "Pack") {
                Pack *packPtr = dynamic_cast<Pack *>(encounter.get());
                counter += packPtr->countBalrogsCount();
            }
        }
    }
    return counter;
}

void Pack::make_move(Player &player) {
    Encounter::make_move(player);
    combatPower += (countBalrogsCount() * 2);
}


string Pack::getDescription() const {
    return "Pack of " + to_string(size) + " members (power " +
           to_string(combatPower) + ", loot "
           + to_string(loot) + ", damage " + to_string(damage) + ")";
}

