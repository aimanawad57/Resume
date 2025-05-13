#include "UnionFind.h"


Node<int, Team> *UnionFind::findTeam(int id) {
    Node<int, Team> *teamNode = teams.find(id);
    if (!teamNode) {
        return nullptr;
    }
    return pathCompress(teamNode);
}


Node<int, Team> *UnionFind::findJockey(int id) {
    Node<int, Jockey> *jockeyNode = jockies.find(id);
    if (!jockeyNode) {
        return nullptr;
    }

    Node<int, Team> *teamNode = jockeyNode->getParent();

    return pathCompress(teamNode); //return the root
}

Node<int, Team> *UnionFind::pathCompress(Node<int, Team> *node) {
    if (!node) {
        return nullptr;
    }
    //find the root
    Node<int, Team> *root = node;
    while (root->getParent() != nullptr) {
        root = root->getParent();
    }

    Node<int, Team> *current = node;
    while (current != root) {
        Node<int, Team> *next = current->getParent();
        current->setParent(root);
        current = next;
    }
    return root;
}

bool UnionFind::sameGroup(int x, int y) {
    if (findJockey(x) == nullptr) {
        return false;
    }
    return findJockey(x) == findJockey(y);
}

hashtable<Jockey> &UnionFind::getJockies() {
    return jockies;
}

hashtable<Team> &UnionFind::getTeams() {
    return teams;
}

Node<int, Team> *UnionFind::unionTeams(int TeamA, int TeamB) {
    if (sameTeam(TeamA, TeamB)) {
        return nullptr; //they are already in the same team
    } else {
        auto teamA = findTeam(TeamA);
        auto teamB = findTeam(TeamB);

        if (teamA->isRemoved() || teamB->isRemoved()) {
            return nullptr;
        }

        int teamSizeA = teamA->getData()->getTeamSize();
        int teamSizeB = teamB->getData()->getTeamSize();
        int recordA = teamA->getData()->getRecord();
        int recordB = teamB->getData()->getRecord();
        if (teamSizeA >= teamSizeB) {
            teamA->getData()->setRecord(recordA + recordB);
            teamA->getData()->setTeamSize(teamSizeA + teamSizeB);
            if (recordA >= recordB) {
                teamB->setParent(teamA);
                teamB->remove();
                return teamA;
            } else {
                reHash(teamB->getKey(), teamA->getKey());
                teamB->setParent(teamA);
                teamB->remove();
                return teamA;
            }

        } else {
            teamB->getData()->setRecord(recordA + recordB);
            teamB->getData()->setTeamSize(teamSizeA + teamSizeB);
            if (recordB > recordA) {
                teamA->setParent(teamB);
                teamA->remove();
                return teamB;
            } else {
                reHash(teamA->getKey(), teamB->getKey());
                teamA->setParent(teamB);
                teamA->remove();
                return teamB;
            }
        }
    }
}

UnionFind::UnionFind() : jockies(), teams(), records() {}

hashtable<Record> &UnionFind::getRecord() {
    return records;
}

bool UnionFind::isOnlyRecord(int record) {
    int index = records.hashFunction(record, records.getSize());
    int counter = 0;

    auto current = records.getTable()[index].getFirst();
    while (current) {
        if (current->getKey() == record && !(current->isRemoved())) {
            counter++;
            if (counter > 1) {
                return false;
            }
        }

        current = current->getNext();
    }

    if (counter == 1) {
        return true;
    }
    return false;
}

bool UnionFind::sameTeam(int x, int y) {
    if (findTeam(x) == nullptr) {
        return false;
    }
    return findTeam(x) == findTeam(y);
}

bool UnionFind::reHash(int newId, int oldId) {
    int oldIndex = teams.hashFunction(oldId, teams.getSize());//teamA
    int newIndex = teams.hashFunction(newId, teams.getSize());//teamB

    auto oldNode = teams.find(oldId);
    auto newNode = teams.find(newId);

    teams.getTable()[oldIndex].disLocate(oldId);
    teams.getTable()[newIndex].disLocate(newId);

    newNode->setKey(oldId);
    newNode->getData()->setTeamId(oldId);
    oldNode->setKey(newId);
    oldNode->getData()->setTeamId(newId);

    teams.getTable()[oldIndex].addNode(newNode);
    teams.getTable()[newIndex].addNode(oldNode);

    return true;
}



