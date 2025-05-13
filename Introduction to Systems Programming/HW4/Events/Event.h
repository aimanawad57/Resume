
#pragma once

#include "../Players/Player.h"
#include "../Players/Job.h"

#include <vector>
#include <iostream>

using std::string;
using std::vector;

class Event {
protected:
    string description;
public:
    explicit Event(const string &description);
    /**
     * Gets the description of the event
     *
     * @return - the description of the event
    */
    virtual string getDescription() const = 0;
    string getEventName() const;


    virtual void make_move(Player &player) = 0;

    virtual ~Event() = default;

};

/** The main class to separate RareEvents from Encounter events **/
class RareEvent : public Event {

public:
    explicit RareEvent(const string &description);

    string getDescription() const override;


    virtual ~RareEvent() = default;
};

/**
* Start adding the Rare Events we have in the game.
*/

class SolarEclipse : public RareEvent {
public:
    SolarEclipse();

    void make_move(Player &player) override;
};

class PotionsMerchant : public RareEvent {
public:
    PotionsMerchant();

    void make_move(Player &player) override;
};

class Encounter : public Event {
protected:
    unsigned int combatPower;
    unsigned int loot;
    unsigned int damage;

public:
    int getCombatPower() const;
    int getLoot() const;
    int getDamage() const;
    string getDescription() const override;
    /* the function is virtual and overridden to make it default for all and,
       if any derived class acts diff it can override it.                    */
    void make_move(Player &player) override;
    Encounter(const string &description, unsigned int combatPower, unsigned int loot,
              unsigned int damage);
};

class Snail : public Encounter {

public:
    Snail();


};

class Slime : public Encounter {

public:
    Slime();
};

class Balrog : public Encounter {

public:
    Balrog();

    void make_move(Player &player) override;
};

class Pack : public Encounter {
private:
    vector<unique_ptr<Encounter>> pack;
    int size;
public:
    string getDescription() const override;
    Pack();
    void make_move(Player &player) override;
    int countBalrogsCount();
    void addEvent(unique_ptr<Encounter> event);
};

