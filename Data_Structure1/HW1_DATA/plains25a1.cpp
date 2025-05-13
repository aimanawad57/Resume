// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"


Plains::Plains() : nonEmptyHerdsTree(),
                   emptyHerdsTree(),
                   globalHorsesTree(), allTime(0) {

}

Plains::~Plains() {}


StatusType Plains::add_herd(int herdId) {
    //Input validation
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    //Check if herdId already exists in emptyHerdsTree or nonEmptyHerdsTree
    if (emptyHerdsTree.findData(herdId) != nullptr ||
        nonEmptyHerdsTree.findData(herdId) != nullptr) {
        return StatusType::FAILURE; // Herd already exists
    }

    Herd *newHerd = new Herd(herdId);
    if (emptyHerdsTree.insertNode(newHerd) != TreeResult::SUCCESS) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Plains::remove_herd(int herdId) {
    //Input validation
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    //Look for the herd in the emptyHerdsTree
    Herd *herdToRemove = emptyHerdsTree.findData(
            herdId);
    if (!herdToRemove) {
        return StatusType::FAILURE; // Not in emptyHerdsTree means FAILURE
    }

    //Remove the herd from the emptyHerdsTree

    if (emptyHerdsTree.deleteNode(herdId) != TreeResult::SUCCESS) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Plains::add_horse(int horseId, int speed) {
    if (horseId <= 0 || speed <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (globalHorsesTree.findData(horseId) != NULL) {
        return StatusType::FAILURE;
    }

    try {
        Horse *newHorse = new Horse(horseId, speed); // May throw std::bad_alloc
        if (globalHorsesTree.insertNode(newHorse) != TreeResult::SUCCESS) {
            delete newHorse; // Cleanup if insertion fails
            return StatusType::FAILURE;
        }
    } catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR; // Handle memory allocation failure
    } catch (...) {
        return StatusType::FAILURE; // Handle other unexpected exceptions
    }

    return StatusType::SUCCESS;
}

StatusType Plains::join_herd(int horseId, int herdId) {
    // Input validation
    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    try {
        // Find the horse in globalHorsesTree
        Horse *horse = globalHorsesTree.findData(horseId);
        if (!horse) {
            return StatusType::FAILURE; // Horse not found
        }

        // Check if the horse is already in a herd
        if (horse->getHerdId() != -1) {
            return StatusType::FAILURE; // Already in a herd
        }

        // Find the herd in either emptyHerdsTree or nonEmptyHerdsTree
        Herd *herd = emptyHerdsTree.findData(herdId);
        if (!herd) {
            herd = nonEmptyHerdsTree.findData(herdId);
        }
        if (!herd) {
            return StatusType::FAILURE; // Herd not found
        }

        // Add the horse to the herd’s tree
        if (herd->getHorsesTree().insertNode(horse, false) !=
            TreeResult::SUCCESS) {
            return StatusType::ALLOCATION_ERROR; // Failed to insert horse into herd
        }
        horse->setPlace(herd->getSize());
        int newSize = herd->getSize() + 1;
        herd->setSize(newSize);

        // Update the horse’s herdId and joinNum
        horse->setHerdId(herdId);
        horse->setJoinNum(allTime);
        allTime++;

        // Move herd from emptyHerdsTree to nonEmptyHerdsTree if needed
        if (herd->getSize() == 1) {
            //std::cout << herd << std::endl;
            AVLNode<Herd> *theNode = emptyHerdsTree.findNode(herdId);
            Herd *tempHerd = new Herd(herdId);
            theNode->setData(tempHerd);// so the data doesn't get deleted.

            if (emptyHerdsTree.deleteNode(herdId) != TreeResult::SUCCESS) {
                return StatusType::ALLOCATION_ERROR;
            }
            //std::cout << herd << std::endl;
            if (nonEmptyHerdsTree.insertNode(herd) != TreeResult::SUCCESS) {
                return StatusType::ALLOCATION_ERROR;
            }
        }

        return StatusType::SUCCESS;

    } catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR; // Handle memory allocation failure
    } catch (const std::exception &e) {
        return StatusType::FAILURE; // Handle other standard exceptions
    } catch (...) {
        return StatusType::FAILURE; // Handle unexpected exceptions
    }
}


StatusType Plains::follow(int horseId, int horseToFollowId) {
    //Input validation
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId) {
        return StatusType::INVALID_INPUT;
    }

    //Locate both horses in globalHorsesTree
    Horse *horse = globalHorsesTree.findData(horseId);
    Horse *horseToFollow = globalHorsesTree.findData(horseToFollowId);

    if (!horse || !horseToFollow) {
        return StatusType::FAILURE; // One or both horses not found
    }

    //Validate that both horses are in the same herd
    if (horse->getHerdId() != horseToFollow->getHerdId() ||
        horse->getHerdId() == -1) {
        return StatusType::FAILURE;
    }

    //Update the follow relationship
    horse->setFollow(horseToFollow);
    horse->setFollowJoinNum(allTime);
    allTime++;

    return StatusType::SUCCESS;
}

StatusType Plains::leave_herd(int horseId) {
    //Input validation
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    //Locate the horse in globalHorsesTree
    Horse *horse = globalHorsesTree.findData(horseId);
    if (!horse) {
        return StatusType::FAILURE; // Horse not found
    }

    // Check if the horse is currently in a herd
    int herdId = horse->getHerdId();
    if (herdId == -1) {
        return StatusType::FAILURE; // Horse is not in a herd
    }

    //Locate the herd
    Herd *herd = nonEmptyHerdsTree.findData(herdId);
    if (!herd) {
        return StatusType::FAILURE; // Herd not found in nonEmptyHerdsTree
    }

    /*auto herdArray = herd->horses.orderedArray();
    for (int i = 0; i < herd->size; ++i) {
        if (herdArray[i]->place > horse->place) {
            herdArray[i]->place = herdArray[i]->place - 1;
        }
    }
    delete[] herdArray;*/
    //Remove the horse from the herd’s tree
    if (herd->getHorsesTree().deleteNode(horseId) != TreeResult::SUCCESS) {
        return StatusType::FAILURE; // Failed to remove horse from herd
    }
    int newSize = herd->getSize() - 1;
    herd->setSize(newSize);

    //Update the horse's status
    horse->setHerdId(-1);
    horse->setPlace(-1);
    horse->setFollow(NULL);
    horse->setJoinNum(-1);
    horse->setFollowJoinNum(-1);

    //Check if the herd becomes empty
    if (herd->getSize() == 0) {
        // Remove from nonEmptyHerdsTree
        if (nonEmptyHerdsTree.deleteNode(herdId) != TreeResult::SUCCESS) {
            return StatusType::ALLOCATION_ERROR;
        }

        // Add to emptyHerdsTree
        Herd *addHerd = new Herd(herdId);
        if (emptyHerdsTree.insertNode(addHerd) != TreeResult::SUCCESS) {
            return StatusType::ALLOCATION_ERROR;
        }
    }

    return StatusType::SUCCESS;
}

output_t<int> Plains::get_speed(int horseId) {
    //Input validation
    if (horseId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    //Locate the horse in globalHorsesTree
    Horse *horse = globalHorsesTree.findData(horseId);
    if (!horse) {
        return output_t<int>(StatusType::FAILURE);
    }

    return output_t<int>(horse->getSpeed());
}

// Function to check if horseId leads to otherHorseId
output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0 || horseId == otherHorseId) {
        return output_t<bool>(StatusType::INVALID_INPUT);
    }

    auto currentHorse = globalHorsesTree.findData(horseId);
    auto otherHorse = globalHorsesTree.findData(otherHorseId);
    if (!currentHorse || !otherHorse) {
        return output_t<bool>(StatusType::FAILURE);
    }
    if (currentHorse->getHerdId() == -1 ||
        currentHorse->getHerdId() != otherHorse->getHerdId()) {
        return output_t<bool>(false);
    }
    Herd *herd = nonEmptyHerdsTree.findData(currentHorse->getHerdId());
    int counter = 0;
    Horse *h = currentHorse;
    while (h) {
        if (h->getId() == otherHorseId) {
            return true;
        }
        if (counter >= herd->getSize()) {
            return false;
        }
        if (h->getFollows() == nullptr) {
            return false;
        }
        if (h->getFollows()->getHerdId() != herd->getId()) {
            return false;
        }
        if (h->getFollows()->getJoinNum() > h->getFollowJoinNum()) {
            return false;
        }
        h = h->getFollows();
        counter++;
    }
    return false;
}


// Function to check if all horses in a herd can run together
output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) {
        return output_t<bool>(StatusType::INVALID_INPUT);
    }

    // Locate the herd in nonEmptyHerdsTree
    Herd *herd = nonEmptyHerdsTree.findData(herdId);
    if (!herd || herd->getSize() == 0) {
        return output_t<bool>(StatusType::FAILURE);
    }

    int herdSize = herd->getSize();
    if (herdSize == 1) {
        return output_t<bool>(true);
    }

    // Check for a single leader
    Horse **herdHorses = herd->getHorsesTree().orderedArray();
    int leaderCount = 0;
    Horse *h = nullptr;

    for (int i = 0; i < herdSize; ++i) {
        if (!herdHorses[i]->isFollowing()) {
            leaderCount++;
            h = herdHorses[i];
        }
        herdHorses[i]->setVisited(false);
        herdHorses[i]->setCanRunTogether(false);
    }

    if (leaderCount != 1) {
        delete[] herdHorses;
        return output_t<bool>(false);
    }
    h->setCanRunTogether(true);
    for (int i = 0; i < herdSize; i++) {
        Horse *h1 = herdHorses[i];
        while (true) {
            if (h1->canRunTogether()) {
                Horse *h2 = herdHorses[i];
                while (!h2->canRunTogether()) {
                    h2->setCanRunTogether(true);
                    h2->setVisited(false);
                    h2 = h2->getFollows();
                }
                break;
            } else if (h1->isVisited()) {
                delete herdHorses;
                return false;
            } else if (h1->getFollows() == nullptr) {
                delete herdHorses;
                return false;
            } else if (h1->getFollows()->getHerdId() != herd->getId()) {
                delete herdHorses;
                return false;
            } else if (h1->getFollows()->getJoinNum() >
                       h1->getFollowJoinNum()) {
                delete herdHorses;
                return false;
            }
            h1->setVisited(true);
            h1 = h1->getFollows();
        }
    }
    delete herdHorses;
    return output_t<bool>(true);
}
/////r

