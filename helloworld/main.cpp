#include <Geode.hpp>

class $(MenuLayer) {
	void onMoreGames(cocos2d::CCObject*) {
		FLAlertLayer::create("Geode", "Hello World from my Custom Mod!", "OK")->show(); 
	} 
};
