#include "Job.h"
#include "Player.h"

#include<string>

using std::string;
using std::to_string;


Player::Player(const string &name, unique_ptr<Job> job,
               unique_ptr<Character> character) : playerName(
        name),
                                                  job(std::move(job)),
                                                  character(
                                                          std::move(
                                                                  character)) {
    this->job->initializePlayer(*this);
}


string Player::getName() const {
    return this->playerName;
}

int Player::getLevel() const {
    return level;
}

int Player::getForce() const {
    return (int)force;
}


int Player::getCoins() const {
    return (int)coins;
}


int Player::getHealthPoints() const {
    return (int)currentHP;
}

int Player::getMaxHealthPoints() const {
    return (int)maxHP;
}

void Player::setHealthPoints(unsigned int healthPoints) {
    this->currentHP = healthPoints;
}

void Player::setMaxHealthPoints(unsigned int maxHealthPoints) {
    this->maxHP = maxHealthPoints;
}

void Player::setLevel(unsigned int playerLevel) {
    this->level = (int)playerLevel;
}

void Player::setForce(int playerForce) {
    if (playerForce <= 0) {
        force = 0;
    }
    else {
        force = playerForce;
    }
}

void Player::setCoins(unsigned int playerCoins) {
    this->coins = playerCoins;
}

string Player::getJob() const {
    return this->job->getJobDescription();
}

IsPlayerMagician Player::getIsPlayerMagician() const {
    return this->job->getMagician();
}


int Player::getCombatPower() const {
    return this->combatPower;
}

void Player::setCombatPower(int power) {
    this->combatPower = power;
}

void Player::updateCombatPower() {
    this->job->updateCombatPower(*this);
}

Range Player::getRange() const {
    return this->job->getRange();
}

string Player::getCharacter() const {
    return this->character->getCharacterDescription();
}

string Player::getDescription() const {
    return playerName + ", " + this->getJob() + " with " +
           this->getCharacter() + " character " +
           "(level " + to_string(level) + ", force " + to_string(force) + ")";

}

void Player::battle(Encounter &encounter) {
    this->job->battle(*this, encounter);
}

bool comparePlayers(const shared_ptr<Player> &player1,
                    const shared_ptr<Player> &player2) {
    if (player1->getLevel() != player2->getLevel()) {
        return player1->getLevel() > player2->getLevel();
    }

    if (player1->getCoins() != player2->getCoins()) {
        return player1->getCoins() > player2->getCoins();
    }
    else {
        return player1->getName() < player2->getName();
    }

}