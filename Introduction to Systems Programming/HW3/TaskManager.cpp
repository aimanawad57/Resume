#include"TaskManager.h"
#include<ostream>


TaskManager::TaskManager() : size(0), TaskManager_id(0) {}

Person *TaskManager::findPerson(const std::string &personName) {
    for (int i = 0; i < size; i++) {
        if (persons[i].getName() == personName) {
            return &persons[i];
        }
    }
    return nullptr;

}

void TaskManager::assignTask(const std::string &personName, const Task &task) {
    Person *person;
    person = findPerson(personName);
    if (person == nullptr) {
        if (size == MAX_PERSONS) {
            throw std::runtime_error(
                    "person not found ,Max size of persons" + personName);
        } else {
            persons[size] = Person(personName);
            person = &persons[size];
            size++;
        }
    }
    Task newTask(task.getPriority(), task.getType(), task.getDescription());
    newTask.setId(TaskManager_id);
    TaskManager_id++;
    person->assignTask(newTask);


}


void TaskManager::completeTask(const std::string &personName) {
    Person *person = findPerson(personName);
    if (person == nullptr) {
        return;
    }
    try {
        person->completeTask();
    } catch (const std::runtime_error &error) {
        throw;
    }
}


void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    if (priority <= 0) {
        return;
    }
    for (int i = 0; i < size; i++) {
        SortedList<Task> updatedTasks = persons[i].getTasks().apply(
                [type, priority](const Task &task) {
                    if (task.getType() == type) {
                        int newPriority =
                                task.getPriority() + priority > 100 ? 100 :
                                task.getPriority() +
                                priority;
                        Task newTask(newPriority, task.getType(),
                                     task.getDescription());
                        newTask.setId(task.getId());
                        return newTask;
                    }
                    return task;

                });
        persons[i].setTasks(updatedTasks);
    }
}


void TaskManager::printAllEmployees() const {
    for (int i = 0; i < size; i++) {
        std::cout << persons[i] << std::endl;
    }
}

void TaskManager::printTasksByType(TaskType type) const {
    SortedList<Task> allTasks;
    for (int i = 0; i < size; i++) {
        for (SortedList<Task>::ConstIterator it = persons[i].getTasks().begin(); it != persons[i].getTasks().end(); ++it) {
            allTasks.insert(*it);
        }
    }
    SortedList<Task> allTypeTasks = allTasks.filter(
            [type](const Task &task) { return task.getType() == type; });
    for (SortedList<Task>::ConstIterator it = allTypeTasks.begin(); it != allTypeTasks.end(); ++it) {
        std::cout << *it << std::endl;
    }
}


void TaskManager::printAllTasks() const {
    SortedList<Task> allTasks;
    for (int i = 0; i < size; i++) {
        for (SortedList<Task>::ConstIterator it = persons[i].getTasks().begin(); it != persons[i].getTasks().end(); ++it) {
            allTasks.insert(*it);
        }
    }
    for (SortedList<Task>::ConstIterator it = allTasks.begin(); it != allTasks.end(); ++it) {
        std::cout << *it << std::endl;
    }
}
