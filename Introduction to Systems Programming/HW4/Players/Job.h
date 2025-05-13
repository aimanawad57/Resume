#pragma once

#include"Player.h"
#include<string>


using std::string;

class Encounter;

class Job {
protected:
    Range range;
    string description;
    IsPlayerMagician magicianPlayer;
public:
    Job(Range range, string description, IsPlayerMagician magicianPlayer);
    void initializeFields(Player &player, int level, int coins, int force,
                          int maxHealth, int combatPower);
    virtual ~Job() = default;
    string getJobDescription() const;
    virtual void initializePlayer(Player &player);
    Range getRange() const;
    IsPlayerMagician getMagician() const;
    virtual void updateCombatPower(Player &player);
    virtual void battle(Player &player, Encounter &encounter);
};


class Warrior : public Job {
public:
    Warrior();
    void initializePlayer(Player &player) override;
    void updateCombatPower(Player &player) override;
};


class Archer : public Job {
public:
    Archer();
    void initializePlayer(Player &player) override;

};


class Magician : public Job {
public:
    Magician();
};


class Character {
protected:
    string description;
public:
    explicit Character(const string &description);
    string getCharacterDescription() const;
};


class Responsible : public Character {
public:
    Responsible();

};

class RiskTaking : public Character {
public:
    RiskTaking();
};