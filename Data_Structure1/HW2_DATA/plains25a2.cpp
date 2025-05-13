// You can edit anything you want in this file.
// However, you need to implement all public Plains function, as provided below as a template.

#include "plains25a2.h"

Plains::Plains()
        : res() {}

Plains::~Plains() {}

StatusType Plains::merge_teams(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    auto team1 = res.getTeams().find(teamId1);
    auto team2 = res.getTeams().find(teamId2);

    if (!(team1) || !(team2) || team1->isRemoved() || team2->isRemoved()) {
        return StatusType::FAILURE;
    }

    int team1Record = team1->getData()->getRecord();
    int team2Record = team2->getData()->getRecord();


    try {
        auto domTeam = res.unionTeams(teamId1, teamId2);

        if (domTeam == nullptr) {
            return StatusType::FAILURE;
        }

        res.getRecord().removeRecord(team1Record, teamId1);
        res.getRecord().removeRecord(team2Record, teamId2);
        if (domTeam->getKey() == teamId1) {
            auto record = res.getRecord().insertRecord(
                    team1Record + team2Record, teamId1);
            record->setParent(team1);
        } else if (domTeam->getKey() == teamId2) {
            auto record = res.getRecord().insertRecord(
                    team1Record + team2Record, teamId2);
            record->setParent(team2);
        }
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType Plains::unite_by_record(int record) {
    if (record <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (!(res.isOnlyRecord(record)) || !(res.isOnlyRecord(-record))) {
        return StatusType::FAILURE;
    }

    auto record1 = res.getRecord().find(record);
    auto record2 = res.getRecord().find((-record));

    auto team1 = res.getTeams().find(record1->getData()->getTeamId());
    auto team2 = res.getTeams().find(record2->getData()->getTeamId());

    if (!team1 || !team2) {
        return StatusType::FAILURE;
    }
    try {
        int deletedKey1 = team1->getKey();
        int deletedKey2 = team2->getKey();

        auto modifiedT = res.unionTeams(team1->getKey(), team2->getKey());

        if (modifiedT == nullptr) {
            return StatusType::FAILURE;
        }

        res.getRecord().removeRecord(record, deletedKey1);
        res.getRecord().removeRecord(-record, deletedKey2);
        auto temp = res.getRecord().insertRecord(0, modifiedT->getKey());
        temp->setParent(modifiedT);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;

    }
    return StatusType::SUCCESS;
}

output_t<int> Plains::get_jockey_record(int jockeyId) {
    if (jockeyId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        auto temp = res.getJockies().find(jockeyId);
        if (temp == nullptr) {
            return StatusType::FAILURE;
        }
        return temp->getData()->getRecord();
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Plains::get_team_record(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    try {
        auto temp = res.getTeams().find(teamId);
        if (temp == nullptr || temp->isRemoved()) {
            return StatusType::FAILURE;
        }
        return temp->getData()->getRecord();
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::add_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto temp = res.getTeams().find(teamId);
    if (temp) {
        return StatusType::FAILURE;
    }
    try {
        auto newTeam = make_shared<Team>(teamId);//chick this
        auto teamNode = res.getTeams().insert(teamId, newTeam);
        auto recordNode = res.getRecord().insertRecord(0, teamId);
        recordNode->setParent(teamNode);
        return StatusType::SUCCESS;

    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::add_jockey(int jockeyId, int teamId) {
    if (jockeyId <= 0 || teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto teamNode = res.getTeams().find(teamId);
    if (!teamNode || teamNode->isRemoved()) {
        return StatusType::FAILURE;
    }
    auto temp = res.getJockies().find(jockeyId);
    if (temp) {
        return StatusType::FAILURE;
    }
    try {
        auto jockey = make_shared<Jockey>(jockeyId);
        auto jockeyNode = res.getJockies().insert(jockeyId, jockey);
        teamNode->getData()->setTeamSize(
                teamNode->getData()->getTeamSize() + 1);
        jockeyNode->setParent(teamNode);
        return StatusType::SUCCESS;
    } catch (...) {
        return StatusType::FAILURE;
    }

}

StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId) {
    if (victoriousJockeyId <= 0 || losingJockeyId <= 0 ||
        victoriousJockeyId == losingJockeyId) {
        return StatusType::INVALID_INPUT;
    }
    auto winner = res.getJockies().find(victoriousJockeyId);
    auto loser = res.getJockies().find(losingJockeyId);
    if (!winner || !loser) {
        return StatusType::FAILURE;
    }
    if (res.sameGroup(victoriousJockeyId, losingJockeyId)) {
        return StatusType::FAILURE;
    }
    try {
        int winnerOldR = winner->getData()->getRecord();
        int loserOldR = loser->getData()->getRecord();
        winner->getData()->setRecord(++winnerOldR);
        loser->getData()->setRecord(--loserOldR);
        auto winnerTeam = res.findJockey(victoriousJockeyId);
        auto loserTeam = res.findJockey(losingJockeyId);
        if (!winnerTeam || !loserTeam) {
            return StatusType::FAILURE;
        }
        int teamRecordW = winnerTeam->getData()->getRecord();
        int teamRecordL = loserTeam->getData()->getRecord();
        int teamWinnerId = winnerTeam->getKey();
        int teamLoserId = loserTeam->getKey();
        if (!(res.getRecord().removeRecord(teamRecordW, teamWinnerId))) {
            return StatusType::FAILURE;
        }
        if (!(res.getRecord().removeRecord(teamRecordL, teamLoserId))) {
            return StatusType::FAILURE;

        }
        winnerTeam->getData()->setRecord(++teamRecordW);
        loserTeam->getData()->setRecord(--teamRecordL);
        auto newRecordWinner = res.getRecord().insertRecord(teamRecordW,
                                                            teamWinnerId);
        auto newRecordLoser = res.getRecord().insertRecord(teamRecordL,
                                                           teamLoserId);
        if (!newRecordWinner) {
            return StatusType::FAILURE;
        }
        if (!newRecordLoser) {
            return StatusType::FAILURE;
        }
        newRecordWinner->setParent(winnerTeam);
        newRecordLoser->setParent(loserTeam);
        return StatusType::SUCCESS;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}