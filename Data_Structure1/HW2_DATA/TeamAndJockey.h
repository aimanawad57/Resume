//
// Created by USER on 21/01/2025.
//

#ifndef DS_WET2_WINTER_2024_2025_TEAMANDJOCKEY_H
#define DS_WET2_WINTER_2024_2025_TEAMANDJOCKEY_H

//JockeyClass
class Jockey {
private:
    int jockeyId;
    int record;

public:
    explicit Jockey(int id) : jockeyId(id), record(0) {}

    int getId() const {
        return jockeyId;
    }

    int getRecord() const {
        return record;
    }

    void setRecord(int m_record) {
        this->record = m_record;
    }
};

//TeamClass


class Team {
private:
    int teamId;
    int totalRecord;
    int teamSize;
public:
    explicit Team(int Id) : teamId(Id), totalRecord(0), teamSize(0) {}

    int getTeamId() const {
        return teamId;
    }

    void setTeamId(int newTeamId) {
        teamId = newTeamId;
    }

    int getRecord() const {
        return totalRecord;
    }

    void setRecord(int m_record) {
        this->totalRecord = m_record;
    }

    int getTeamSize() const {
        return teamSize;
    }

    void setTeamSize(int size) {
        teamSize = size;
    }

    void copyWithoutId(const Team &other) {
        totalRecord = other.totalRecord;
        teamSize = other.teamSize;
    }
};

class Record {
    int teamId;
public:
    Record(int teamId) : teamId(teamId) {}

    int getTeamId() const {
        return teamId;
    }

    bool operator==(int id) const {
        return this->teamId == id;
    }

    explicit operator int() const {
        return teamId;
    }
};


#endif //DS_WET2_WINTER_2024_2025_TEAMANDJOCKEY_H
