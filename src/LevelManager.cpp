#include "./LevelManager.hpp"

LevelManager::LevelManager() {
    // fetch and store levels

    m_listener.bind([this] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            auto _res = res->string().unwrapOr("Uh oh!");
            if (_res == "Uh oh!") return log::info("The request could not be stringified... So sad :(");

            auto index = _res.find("{");
            if (index == std::string::npos) return log::info("The request could not be stringified... So sad :(");

            auto _result = _res.substr(index);
            auto result = matjson::parse(_result.substr(0, _result.length() - 2));
            if (result.isErr()) return log::info("The results could not be parsed into JSON... So sad :(");

            auto unwrappedResult = result.unwrap();
            if (!unwrappedResult.contains("table")) return log::info("The results did not have a 'table' key... So sad :(");
            if (!unwrappedResult["table"].contains("rows")) return log::info("The results had a 'table' key, but not a 'rows' key... So sad :(");
            // auto rows = result["table"].unwrap()["rows"].as_array().unwrap();

            auto rows = unwrappedResult["table"]["rows"].asArray();
            if (rows.isErr()) return log::info("The 'rows' key could not become a matjson array... So sad :(");

            for (auto& row : rows.unwrap()) {
                if (!row.contains("c")) {
                    log::info("this row does not have a 'c' key... So sad :(");
                    continue;
                }
                auto stillWrappedContents = row["c"].asArray();
                if (stillWrappedContents.isErr()) {
                    log::info("this row's contents were not formed into an array... So sad :(");
                    continue;
                }

                auto contents = stillWrappedContents.unwrap();
                if (contents.size() < 2 || !contents[0].contains("v") || !contents[1].contains("f")) {
                    log::info("this row's has fewer than two elements OR has no level name OR no level ID... So sad :(");
                    continue;
                }
                
                auto levelName = contents[0]["v"].asString();
                if (levelName.isErr()) return log::info("levelName could not be parded into a string... So sad :(");
                auto levelID = contents[1]["f"].asString();
                if (levelID.isErr()) return log::info("levelID could not be parded into a string... So sad :(");
                levels[geode::utils::string::toLower(levelName.unwrap())] = levelID.unwrap();
            }
            
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
