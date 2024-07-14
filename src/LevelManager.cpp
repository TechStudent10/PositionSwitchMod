#include "./LevelManager.hpp"

LevelManager::LevelManager() {
    // fetch and store levels

    m_listener.bind([this] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            auto _result = res->string().unwrapOr("Uh oh!").substr(47);
            auto result = matjson::parse(_result.substr(0, _result.length() - 2));
            auto rows = result["table"]["rows"].as_array();
            for (auto& row : rows) {
                auto contents = row["c"].as_array();
                
                auto levelName = geode::utils::string::toLower(contents[0]["v"].as_string());
                auto levelID = contents[1]["f"].as_string();
                levels[levelName] = levelID;
            }
            // log::info("levels inserted! length: {}", levels.size());
            
        } else if (web::WebProgress* p = e->getProgress()) {
            // TODO: make this log toggleable
            // log::info("progress: {}", p->downloadProgress().value_or(0.f));
        } else if (e->isCancelled()) {
            // TODO: make this log toggleable
            log::info("The request was cancelled... So sad :(");
        }
    });

    auto req = web::WebRequest();
    m_listener.setFilter(
        req.get("https://docs.google.com/spreadsheets/d/11J28JcremydDAi6vIcQwKTNa9nfRis8O4soxE7_X5qc/gviz/tq?tqx=out:json&tq&gid=0")
    );

};
