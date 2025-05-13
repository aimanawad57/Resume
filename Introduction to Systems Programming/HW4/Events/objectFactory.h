#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include<vector>


template<typename T>
class Factory {
public:
    using CreationFunction = std::function<std::unique_ptr<T>(
            std::istream &)>;

    static Factory &getInstance();

    void registerType(const std::string &name, CreationFunction createFunc);

    std::unique_ptr<T>
    create(const std::string &name, std::istream &stream) const;

    void registerTypes(
            const std::vector<std::pair<std::string, CreationFunction>> &types);
private:
    std::unordered_map<std::string, CreationFunction> creators;
};

/* The create function for creating the objects */
template<typename Derived, typename Base>
std::unique_ptr<Base> createInstance(std::istream &stream) {
    if(stream.eof()){}
    return std::make_unique<Derived>();
}

/** Factory Implementation **/

template<typename T>
Factory<T> &Factory<T>::getInstance() {
    static Factory<T> instance;
    return instance;
}

template<typename T>
void
Factory<T>::registerType(const std::string &name, CreationFunction createFunc) {
    creators[name] = createFunc;
}

template<typename T>
std::unique_ptr<T>
Factory<T>::create(const std::string &name, std::istream &stream) const {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return (it->second)(stream);
    }
    return nullptr;
}

template<typename T>
void Factory<T>::registerTypes(
        const std::vector<std::pair<std::string, CreationFunction>> &types) {
    for (const auto &type: types) {
        registerType(type.first, type.second);
    }
}