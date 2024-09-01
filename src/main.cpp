#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "./LevelManager.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		LevelManager::get(); // inititalizes the list on startup

		return true;
	}
};

class $modify(MyInfoLayer, LevelInfoLayer) {
	bool init(GJGameLevel* level, bool challenge) {
		if (!LevelInfoLayer::init(level, challenge)) return false;

		auto hasLevel = LevelManager::get().levels.contains(
			geode::utils::string::toLower(level->m_levelName)
		);
		if (hasLevel) {
			auto startPosSprite = CCSprite::create("startpos-btn.png"_spr);
			startPosSprite->setScale(0.625);
			auto btn = CCMenuItemSpriteExtra::create(
				startPosSprite, this, menu_selector(MyInfoLayer::onStartPos)
			);
			btn->setID("startpos-btn"_spr);
			auto menu = this->getChildByIDRecursive("left-side-menu");
			menu->addChild(btn);
			menu->updateLayout();
		}
		
		return true;
	}

	void onStartPos(CCObject*) {
		auto levelName = geode::utils::string::toLower(m_level->m_levelName);
		auto startPosLevels = LevelManager::get().levels;
		auto levelBrowserLayer = LevelBrowserLayer::create(GJSearchObject::create(
			SearchType::Search,
			startPosLevels.at(levelName)
		));
		auto scene = CCScene::create();
		scene->addChild(levelBrowserLayer);
		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(.5f, scene));
	}
};
