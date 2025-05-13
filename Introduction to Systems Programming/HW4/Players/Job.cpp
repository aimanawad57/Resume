#include "Job.h"
#include "Event.h"
#include "Utilities.h"

Job::Job(Range range, string description, IsPlayerMagician magicianPlayer) : range(range),
                                                                             description(
                                                                                     description),
                                                                             magicianPlayer(
                                                                                     magicianPlayer) {}

void Job::initializeFields(Player &player, int level, int coins, int force,
                           int maxHealth, int combatPower) {
    player.setLevel(level);
    player.setCoins(coins);
    player.setForce(force);
    player.setMaxHealthPoints(maxHealth);
    player.setHealthPoints(player.getMaxHealthPoints());
    player.setCombatPower(combatPower);
}

const int DefaultLevel = 1;
const int DefaultCoins = 10;
const int DefaultForce = 5;
const int DefaultMaxHealth = 100;
const int DefaultCombatPower = (DefaultLevel + DefaultForce);

void Job::initializePlayer(Player &player) {
    initializeFields(player, DefaultLevel, DefaultCoins, DefaultForce,
                     DefaultMaxHealth, DefaultCombatPower);
}

void Job::updateCombatPower(Player &player) {
    player.setCombatPower(player.getForce() + player.getLevel());
}

Range Job::getRange() const {
    return range;
}

IsPlayerMagician Job::getMagician() const {
    return magicianPlayer;
}


string Job::getJobDescription() const {
    return this->description;
}

void Job::battle(Player &player, Encounter &encounter) {
    bool result;
    if ((int)player.getCombatPower() > encounter.getCombatPower()) {
        player.setLevel(player.getLevel() + 1);
        player.setCoins(player.getCoins() + encounter.getLoot());
        if (player.getRange() == ShortRange) {
            player.setHealthPoints((player.getHealthPoints() - 10) <= 0 ? 0 :
                                   player.getHealthPoints() - 10);
        }
        player.updateCombatPower();
        result = true;
    } else {
        int health = player.getHealthPoints() - encounter.getDamage();
        if (health <= 0) {
            player.setHealthPoints(0);
        } else {
            player.setHealthPoints(health);
        }
        result = false;
    }
    string outcome;
    if (result) {
        outcome = getEncounterWonMessage(player, encounter.getLoot());
    } else {
        outcome = getEncounterLostMessage(player, encounter.getDamage());
    }
    printTurnOutcome(outcome);
}

/*warrior job
 *
 *
 *
 */

const int WarriorMaxHealth = 150;
const double WarriorForceMultiply = 2.0;
const int WarriorStartingCombatPower = (
        (int)(DefaultForce * WarriorForceMultiply) + DefaultLevel);

void Warrior::initializePlayer(Player &player) {
    initializeFields(player, DefaultLevel, DefaultCoins, DefaultForce,
                     WarriorMaxHealth, WarriorStartingCombatPower);
}

Warrior::Warrior() : Job(ShortRange, "Warrior", IsNotMagician) {}

void Warrior::updateCombatPower(Player &player) {
    player.setCombatPower(
            player.getForce() * WarriorForceMultiply + player.getLevel());
}

/*
 * Archer job
 *
 *
 *
 */

const int ArcherCoins = 20;

void Archer::initializePlayer(Player &player) {
    initializeFields(player, DefaultLevel, ArcherCoins, DefaultForce,
                     DefaultMaxHealth, DefaultCombatPower);
}


Archer::Archer() : Job(LongRange, "Archer", IsNotMagician) {}

/*
 * Magician job
 *
 *
 */

Magician::Magician() : Job(LongRange, "Magician", IsMagician) {}

/*
 * class character
 *
 *
 */

Character::Character(const std::string &description) : description(
        description) {}


string Character::getCharacterDescription() const {
    return this->description;
}

Responsible::Responsible() : Character("Responsible") {}


RiskTaking::RiskTaking() : Character("RiskTaking") {}