#include <memory>
#include <sstream>

#include "MatamStory.h"
#include<vector>
#include "Utilities.h"


using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::unique_ptr;

MatamStory::MatamStory(std::istream &eventsStream,
                       std::istream &playersStream) {
    m_turnIndex = 1;
    initializeFactoriesAndRegister();
    initializeEvents(eventsStream);
    initializePlayersAndBoard(playersStream);

}

void MatamStory::initializePlayersAndBoard(std::istream &playersStream) {
    string name, job, character;
    int numPlayers = 0;
    while (true) {
        if (!(playersStream >> name >> job >> character) || ++numPlayers > 6 ||
            (name.size() < 3 || name.size() > 15)) {
            throw std::runtime_error("Invalid Players File");
        }
        unique_ptr<Job> jobPtr = Factory<Job>::getInstance().create(job,
                                                                    playersStream);
        if (jobPtr == nullptr) {
            throw (std::runtime_error("Invalid Players File"));
        }
        unique_ptr<Character> characterPtr = Factory<Character>::getInstance().create(
                character, playersStream);
        if (characterPtr == nullptr) {
            throw (std::runtime_error(
                    "Invalid Players File"));
        }
        auto player = make_shared<Player>(name, std::move(jobPtr),
                                          std::move(characterPtr));
        players.push_back(player);
        board.addPlayer(player);
        if (playersStream.eof()) {
            break;
        }
    }
}

void MatamStory::initializeEvents(std::istream &eventsStream) {
    string event;
    while (eventsStream >> event) {

        unique_ptr<Event> eventPtr = (
                Factory<Event>::getInstance().create(
                        event, eventsStream));

        if (!eventPtr) {
            throw (std::runtime_error("Invalid Events File"));
        }

        events.push_back(std::move(eventPtr));
        if (eventsStream.eof()) {
            break;
        }
    }
    if (events.size() < 2) {
        throw (std::runtime_error("Invalid Events File"));
    }
    eventIter = events.begin();
}

std::unique_ptr<Encounter> MatamStory::createPack(std::istream &eventStream) {
    int packSize;
    if (!(eventStream >> packSize) || packSize < 2) {
        throw (std::runtime_error("Invalid Events File"));
    }

    auto pack = std::make_unique<Pack>();

    std::string eventType;
    for (int i = 0; i < packSize; ++i) {
        if (!(eventStream >> eventType)) {
            throw (std::runtime_error("Invalid Events File"));
        }
        std::unique_ptr<Event> encounter = Factory<Event>::getInstance().create(
                eventType, eventStream);
        if (!encounter) {
            throw std::runtime_error("Invalid Events File");
        }
        //check if it's an actual encounter event.
        if (dynamic_cast<Encounter *>(encounter.get()) == nullptr) {
            throw std::runtime_error("Invalid Events File");
        }
        std::unique_ptr<Encounter> encounterPtr(
                dynamic_cast<Encounter *>(encounter.release()));
        if (!encounterPtr) {
            throw std::runtime_error("Invalid Events File");
        }
        /* need to cast to Encounter unique ptr */
        pack->addEvent(std::move(encounterPtr));
    }

    return pack;
}


bool MatamStory::registerJobs() {
    Factory<Job>::getInstance().registerTypes(
            {
                    {"Warrior",  createInstance<Warrior, Job>},
                    {"Magician", createInstance<Magician, Job>},
                    {"Archer",   createInstance<Archer, Job>}
            });
    return true;
}

bool MatamStory::registerCharacters() {
    Factory<Character>::getInstance().registerTypes(
            {
                    {"RiskTaking",  createInstance<RiskTaking, Character>},
                    {"Responsible", createInstance<Responsible, Character>}
            });
    return true;
}

bool MatamStory::registerEvents() {
    Factory<Event>::getInstance().registerTypes(
            {
                    // Rare Events
                    {"SolarEclipse",    createInstance<SolarEclipse, Event>},
                    {"PotionsMerchant", createInstance<PotionsMerchant, Event>},

                    // Encounter Events
                    {"Balrog",          createInstance<Balrog, Event>},
                    {"Snail",           createInstance<Snail, Event>},
                    {"Slime",           createInstance<Slime, Event>},
                    {"Pack",            [this](
                            std::istream &stream) -> std::unique_ptr<Event> {
                        return this->createPack(stream);
                    }}
            });
    return true;
}

bool MatamStory::initializeFactoriesAndRegister() {
    //initialize all the factories needed in the game.
    jobFactory = &(Factory<Job>::getInstance());
    characterFactory = &(Factory<Character>::getInstance());
    eventFactory = &(Factory<Event>::getInstance());

    //register the required types of inputs.
    registerJobs();
    registerCharacters();
    registerEvents();

    return true;
}

void MatamStory::playRound() {
    printRoundStart();
    auto it = players.begin();
    while (it != players.end()) {
        auto &player = *it;
        if (player->getHealthPoints() <= 0) {
            it = players.erase(it);
        }
        else {
            printTurnDetails(m_turnIndex++, *player, *(*eventIter));
            (*eventIter)->make_move(*player);
            it++;
            eventIter++;
            if (eventIter == events.end()) {
                eventIter = events.begin();
            }
        }
    }

    printRoundEnd();
    board.printLeaderBoard();
    printBarrier();
}


bool MatamStory::isGameOver() const {
    const vector<shared_ptr<Player>> playersInBoard = board.getPlayersBoard();
    bool playerAlive = false;
    for (const shared_ptr<Player> &player: playersInBoard) {
        if (player->getLevel() >=
            10) {//this will take place in the top players.
            printGameOver();
            printWinner(*player);
            return true;
        }
        if (player->getHealthPoints() >
            0) {//if any players is not 0, mean game continues.
            playerAlive = true;
        }
    }
    if (!playerAlive) {
        printGameOver();
        printNoWinners();
        return true;
    }
    return false; //there are playerAlive game continue
}

void MatamStory::play() {
    printStartMessage();
    int index = 1;
    for (const auto &player: players) {
        printStartPlayerEntry(index++, *player);
    }
    printBarrier();
    while (!isGameOver()) {
        playRound();
    }


}

void Leaderboard::addPlayer(shared_ptr<Player> &player) {
    players_board.push_back(player);
    sortBoard();
}

void Leaderboard::sortBoard() {
    std::sort(players_board.begin(), players_board.end(), comparePlayers);
}

const vector<shared_ptr<Player>> &Leaderboard::getPlayersBoard() const {
    return players_board;
}

void Leaderboard::printLeaderBoard() {
    sortBoard();
    printLeaderBoardMessage();
    int index = 1;
    for (auto &player: players_board) {
        printLeaderBoardEntry(index++, *player);
    }
}


