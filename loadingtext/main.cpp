#include <Geode.hpp>

USE_GEODE_NAMESPACE();

class $modify(LoadingLayer) {
    bool init(bool fromReload) {
        if (!$LoadingLayer::init(fromReload))
            return false;
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        getChild<CCNode*>(this, 1)->removeFromParent();

        auto spr = CCSprite::create("loadingImage.png");
        spr->setPosition({ winSize.width / 2, winSize.height / 2 + 5.f });
        spr->setScale(1.2f);
        this->addChild(spr);

        return true;
    }

	const char* getLoadingString() {
		return "You should finish loading... NOW";
	}
};
