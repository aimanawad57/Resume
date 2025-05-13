
#pragma once

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;
using std::unique_ptr;

class Job;

class Character;

enum IsPlayerMagician {
    IsMagician,
    IsNotMagician
};

class Encounter;

enum Range {
    ShortRange, // Enum value for short range
    LongRange   // Enum value for long range
};


class Player {
public:

    Player(const string &name, unique_ptr<Job> job,
           unique_ptr<Character> character);
    /**
     * Gets the description of the player
     *
     * @return - description of the player
    */
    string getDescription() const;

    /**
     * Gets the name of the player
     *
     * @return - name of the player
    */
    string getName() const;

    /**
     * Gets the current level of the player
     *
     * @return - level of the player
    */
    int getLevel() const;


    void setLevel(unsigned int playerLevel);

    /**
     * Gets the of force the player has
     *
     * @return - force points of the player
    */
    int getForce() const;

    void setForce(int playerForce);


    /**
     * Gets the amount of health points the player currently has
     *
     * @return - health points of the player
    */
    int getHealthPoints() const;


    void setHealthPoints(unsigned int healthPoints);

    int getMaxHealthPoints() const;

    void setMaxHealthPoints(unsigned int maxHealthPoints);

    string getCharacter() const;


    /**
     * Gets the amount of coins the player has
     *
     * @return - coins of the player
    */
    int getCoins() const;


    void setCoins(unsigned int playerCoins);


    string getJob() const;

    void setCombatPower(int power);

    int getCombatPower() const;

    Range getRange() const;

    IsPlayerMagician getIsPlayerMagician() const;

    void updateCombatPower();

    void battle(Encounter &encounter);
private:
    string playerName;
    int level;
    unsigned int force;
    unsigned int maxHP;
    unsigned int currentHP;
    unsigned int coins;
    unsigned int combatPower;
    unique_ptr<Job> job;
    unique_ptr<Character> character;


};

bool
comparePlayers(const shared_ptr<Player> &player1,
               const shared_ptr<Player> &player2);