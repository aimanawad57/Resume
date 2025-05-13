#ifndef ENTITIES_H
#define ENTITIES_H

#include "AVLTree.h"
#include "wet1util.h"

class Horse {
private:
    int horseId;
    int speed;
    int herdId;
    int joinNum; // Counter incremented each time the horse joins a herd
    int followJoinNum; // The joinNum of the horse this horse is following
    int place;
    Horse *follows;
    bool visited; // Flag to track if the horse has been processed (will help us in the can_run_together function)
    bool can_run_togther;

public:

    explicit Horse(int id, int spd);

    bool operator<(const Horse &other) const;
    bool operator>(const Horse &other) const;
    bool operator==(const Horse &other) const;

    bool operator<(const int id) const;
    bool operator>(const int id) const;
    bool operator==(const int id) const;

    bool isFollowing() const;

    int getId() const;
    int getSpeed() const;
    int getHerdId() const;
    int getJoinNum() const;
    int getFollowJoinNum() const;
    int getPlace() const;
    Horse *getFollows() const;
    bool isVisited() const;
    bool canRunTogether() const;

    void setPlace(int new_place);
    void setHerdId(int new_herd_id);
    void setJoinNum(int new_join_num);
    void setFollow(Horse *new_follow);
    void setFollowJoinNum(int new_follow_join_num);
    void setVisited(bool v);
    void setCanRunTogether(bool v);

    operator int() const;

};

class Herd {
private:
    int herdId;
    int size;
    AVLTree<Horse> horses; // AVL tree of pointers to Horse objects in this herd
public:

    explicit Herd(int id);

    bool operator<(const Herd &other) const;
    bool operator>(const Herd &other) const;
    bool operator==(const Herd &other) const;

    bool operator<(const int id) const;
    bool operator>(const int id) const;
    bool operator==(const int id) const;

    int getId() const;
    int getSize() const;
    AVLTree<Horse> &getHorsesTree();

    void setSize(int newSize);
};

#endif //ENTITIES_H