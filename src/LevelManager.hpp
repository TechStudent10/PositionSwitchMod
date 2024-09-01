#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

class LevelManager {
private:
    EventListener<web::WebTask> m_listener;

    LevelManager();

public:
    std::map<std::string, std::string> levels;
    static LevelManager& get() {
        static LevelManager instance;
        return instance;
    }
};
 