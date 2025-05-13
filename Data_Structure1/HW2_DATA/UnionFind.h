#ifndef MIVNE_WET_2_UNIONFIND_H
#define MIVNE_WET_2_UNIONFIND_H

#include "hash_table.h"
#include "TeamAndJockey.h"

class UnionFind {
private:
    hashtable<Jockey> jockies;
    hashtable<Team> teams;
    hashtable<Record> records;

public:
    UnionFind();
    Node<int, Team> *findTeam(int id);
    Node<int, Team> *findJockey(int id);
    Node<int, Team> *pathCompress(Node<int, Team> *node);
    bool sameGroup(int x, int y);
    bool sameTeam(int x, int y);
    hashtable<Jockey> &getJockies();
    hashtable<Team> &getTeams();
    hashtable<Record> &getRecord();
    Node<int, Team> *unionTeams(int teamA, int teamB);
    bool isOnlyRecord(int record);
    bool reHash(int newId, int oldId);

};

#endif //MIVNE_WET_2_UNIONFIND_H
