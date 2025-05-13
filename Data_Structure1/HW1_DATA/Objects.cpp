#include "Objects.h"

// ------------------ Horse Implementation ------------------ //
static const int DefaultId = -1;
static const int Zero = 0;


Horse::Horse(int id, int spd)
        :
        horseId(id),
        speed(spd),
        herdId(DefaultId),
        joinNum(Zero),
        followJoinNum(Zero),
        place(DefaultId),
        follows(nullptr),
        visited(false), can_run_togther(false) {}


bool Horse::operator<(const Horse &other) const {
    return horseId < other.horseId;
}

bool Horse::operator>(const Horse &other) const {
    return horseId > other.horseId;
}

bool Horse::operator==(const Horse &other) const {
    return horseId == other.horseId;
}

bool Horse::operator<(const int id) const {
    return horseId < id;
}

bool Horse::operator>(const int id) const {
    return !((*this) < id);
}

bool Horse::operator==(const int id) const {
    return horseId == id;
}

int Horse::getId() const {
    return horseId;
}

int Horse::getPlace() const {
    return place;
}

int Horse::getFollowJoinNum() const {
    return followJoinNum;
}

int Horse::getHerdId() const {
    return herdId;
}

int Horse::getJoinNum() const {
    return joinNum;
}

int Horse::getSpeed() const {
    return speed;
}

Horse::operator int() const {
    return horseId;
}

bool Horse::canRunTogether() const {
    return can_run_togther;
}

bool Horse::isVisited() const {
    return visited;
}

Horse *Horse::getFollows() const {
    return follows;
}

bool Horse::isFollowing() const {
    if (this->follows) {
        if (followJoinNum > follows->joinNum && follows->herdId == herdId &&
            herdId != -1) {
            return true;
        }
    }
    return false;
}

void Horse::setPlace(int new_place) {
    this->place = new_place;
}

void Horse::setHerdId(int new_herd_id) {
    this->herdId = new_herd_id;
}

void Horse::setJoinNum(int new_join_num) {
    this->joinNum = new_join_num;
}

void Horse::setFollow(Horse *new_follow) {
    this->follows = new_follow;
}

void Horse::setFollowJoinNum(int new_follow_join_num) {
    this->followJoinNum = new_follow_join_num;
}

void Horse::setVisited(bool v) {
    this->visited = v;
}

void Horse::setCanRunTogether(bool v) {
    this->can_run_togther = v;
}

// ------------------ Herd Implementation ------------------ //

Herd::Herd(int id) : herdId(id), size(0), horses() {}

bool Herd::operator<(const Herd &other) const {
    return herdId < other.herdId;
}

bool Herd::operator>(const Herd &other) const {
    return herdId > other.herdId;
}

bool Herd::operator==(const Herd &other) const {
    return herdId == other.herdId;
}

bool Herd::operator<(const int id) const {
    return herdId < id;
}

bool Herd::operator>(const int id) const {
    return herdId > id;
}

bool Herd::operator==(const int id) const {
    return herdId == id;
}

int Herd::getId() const {
    return herdId;
}

int Herd::getSize() const {
    return size;
}

AVLTree<Horse> &Herd::getHorsesTree() {
    return this->horses;
}

void Herd::setSize(int newSize) {
    this->size = newSize;
}