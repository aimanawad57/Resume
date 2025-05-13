
#pragma once

#include <iostream>
#include <memory>
#include <algorithm>

#include "Players/Player.h"
#include "Events/Event.h"
#include "objectFactory.h"

using std::shared_ptr;


class Leaderboard {
public:
    void addPlayer(shared_ptr<Player> &player);
    void printLeaderBoard();
    const vector<shared_ptr<Player>> &getPlayersBoard() const;
    void sortBoard();

private:
    vector<shared_ptr<Player>> players_board;
};

class MatamStory {

    vector<shared_ptr<Player>> players;
    vector<shared_ptr<Event>> events;
    vector<shared_ptr<Event>>::iterator eventIter;


    Leaderboard board;

    unsigned int m_turnIndex;

    Factory<Job> *jobFactory;
    Factory<Character> *characterFactory;
    Factory<Event> *eventFactory;

    /**
     * Playes a single turn for a player
     *
     * @param player - the player to play the turn for
     *
     * @return - void
    */

    /**
     * Plays a single round of the game
     *
     * @return - void
    */
    void playRound();

    /**
     * Checks if the game is over
     *
     * @return - true if the game is over, false otherwise
    */
    bool isGameOver() const;

    bool registerJobs();

    bool registerCharacters();

    bool registerEvents();

    bool initializeFactoriesAndRegister();

    void initializePlayersAndBoard(std::istream &playersStream);

    void initializeEvents(std::istream &eventsStream);


    std::unique_ptr<Encounter> createPack(std::istream &eventStream);


public:

    /* testing function
    const std::vector<std::shared_ptr<Player>> &getPlayers() const;
    const std::vector<std::shared_ptr<Player>> &getEvents() const;
    */

    /**
     * Constructor of MatamStory class
     *
     * @param eventsStream - events input stream (file)
     * @param playersStream - players input stream (file)
     *
     * @return - MatamStory object with the given events and players
     *
    */
    MatamStory(std::istream &eventsStream, std::istream &playersStream);

    /**
     * Plays the entire game
     *
     * @return - void
    */
    void play();
};

