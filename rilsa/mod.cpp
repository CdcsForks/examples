#include <Geode.hpp>
#include <array>

USE_GEODE_NAMESPACE();

GEODE_API bool GEODE_CALL geode_load(Mod* mod) {
    Interface::get()->init(mod);
    return true;
}

GEODE_API void GEODE_CALL geode_unload() {}

static std::string loadStepToString(int step) {
    if (step < 1) {
        return "GJ_GameSheet";
    }
    if (step >= 14) {
        return "Finished";
    }
    std::array<const char*, 14> steps = {
        "GJ_GameSheet02",
        "GJ_GameSheet03",
        "GJ_GameSheet04",
        "GJ_GameSheetGlow",
        "FireSheet_01, GJ_ShopSheet, smallDot, square02_001",
        "CCControlColourPickerSpriteSheet, GJ_gradientBG, edit_barBG_001, GJ_button_01, slidergroove2, sliderBar2",
        "GJ_square01, GJ_square02, GJ_square03, GJ_square04, GJ_square05, gravityLine_001",
        "chatFont",
        "Robot_AnimDesc, glassDestroy01, coinPickupEffect, explodeEffect",
        "nothing lol",
        "GameManager",
        "CCTextInputNode",
        "CCCircleWave",
        "Finished",
    };
    return steps[step - 1];
}

constexpr int anim_duration = 360;

void drawSector(
    CCDrawNode* node,
    CCPoint origin, float radius, float angle_degree,
    unsigned int num_of_points = 100
) {
    const auto start = origin + CCPoint { radius, 0 };
    const auto angle_step = 2 * M_PI * angle_degree / 360.f / num_of_points;
    std::vector<CCPoint> circle;
    circle.emplace_back(origin);
    for (auto i = 0u; i <= num_of_points; i++)
    {
        auto rads = angle_step * i;
        auto x = origin.x + radius * cosf(rads);
        auto y = origin.y + radius * sinf(rads);
        circle.emplace_back(x, y);
    }
    node->drawPolygon(circle.data(), circle.size(), { 1.f, 1.f, 1.f, 1.f }, 0.f, { 1.f, 1.f, 1.f, 1.f });
}

class RadialClipFull : public CCActionInterval {
protected:
    CCNode* m_node;
    CCDrawNode* m_drawNode;
    bool m_reverse;

    void update(float time) override {
        m_drawNode->setPosition(m_node->getPosition());
        m_drawNode->clear();
        if (time >= 0.9999f) {
            std::vector<CCPoint> points = {
                { -500.f, -300.f },
                { -500.f,  300.f },
                {  500.f,  300.f },
                {  500.f, -300.f },
            };
            m_drawNode->drawPolygon(
                points.data(), points.size(),
                { 1.f, 1.f, 1.f, 1.f }, 0, { 0.f, 0.f, 0.f, 0.f }
            );
        } else {
            float d = 360.f * time;
            if (m_reverse) d = -d;
            drawSector(m_drawNode, { 0, 0 }, 90.f, d);
        }
    }

    bool init(float duration, float from, bool reversed, CCNode* node, CCNode* target, int tag) {
        if (!this->initWithDuration(duration))
            return false;
        
        this->m_node = node;
        this->m_reverse = reversed;

        this->m_drawNode = CCDrawNode::create();
        this->m_drawNode->setRotation(from);
        this->m_drawNode->setPosition(node->getPosition());

        auto clippingNode = CCClippingNode::create(this->m_drawNode);
        clippingNode->addChild(node);
        clippingNode->setTag(tag);
        target->addChild(clippingNode);

        return true;
    }

public:
    static RadialClipFull* create(float duration, float from, bool reversed, CCNode* node, CCNode* target, int tag) {
        auto ret = new RadialClipFull;
        if (ret && ret->init(duration, from, reversed, node, target, tag)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class $modify(CustomLoadingLayer, LoadingLayer) {
    bool init(bool fromReload) {
        this->setUserData(0);

        if (!$LoadingLayer::init(fromReload))
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        this->m_sliderBar->retain();
        this->removeAllChildren();

        auto bg = CCLayerColor::create({ 50, 50, 50, 255 });
        bg->setOpacity(0);
        bg->setTag(1);
        bg->runAction(CCFadeTo::create(4.f, 90));
        this->addChild(bg);


        auto innerSpr = CCSprite::create("geode-logo-inner.png");
        innerSpr->setPosition(this->getContentSize() / 2);
        innerSpr->setOpacity(0);
        innerSpr->setScale(.6f);
        innerSpr->runAction(CCSequence::create(
            CCSpawn::create(
                CCEaseInOut::create(
                    RadialClipFull::create(2.f, -200.f, false, innerSpr, this, 2),
                    2.f
                ),
                CCSequence::create(
                    CCDelayTime::create(.5f),
                    CCSpawn::create(
                        CCFadeTo::create(2.f, 255),
                        nullptr
                    ),
                    nullptr
                ),
                nullptr
            ),
            CCDelayTime::create(.5f),
            CCSpawn::create(
                CCEaseExponentialInOut::create(
                    CCMoveBy::create(2.f, { 0.f, 40.f })
                ),
                CCEaseExponentialInOut::create(
                    CCScaleTo::create(2.f, .5f)
                ),
                nullptr
            ),
            nullptr
        ));

        auto outerSpr = CCSprite::create("geode-logo-outer.png");
        outerSpr->setPosition(this->getContentSize() / 2);
        outerSpr->setOpacity(0);
        outerSpr->setScale(.6f);
        outerSpr->runAction(CCSequence::create(
            CCDelayTime::create(.5f),
            CCSpawn::create(
                CCEaseInOut::create(
                    RadialClipFull::create(2.f, -45.f, true, outerSpr, this, 3),
                    2.f
                ),
                CCSequence::create(
                    CCDelayTime::create(.5f),
                    CCSpawn::create(
                        CCFadeTo::create(2.f, 160),
                        nullptr
                    ),
                    nullptr
                ),
                nullptr
            ),
            CCSpawn::create(
                CCEaseExponentialInOut::create(
                    CCMoveBy::create(2.f, { 0.f, 40.f })
                ),
                CCEaseExponentialInOut::create(
                    CCScaleTo::create(2.f, .5f)
                ),
                nullptr
            ),
            nullptr
        ));


        auto circle = CCSprite::create("geode-loading-circle.png");
        circle->setPosition({
            this->getContentSize().width / 2,
            this->getContentSize().height / 2 + 37.f
        });
        circle->setOpacity(150);
        circle->setScale(.8f);

        auto drawNode = CCDrawNode::create();
        drawNode->setRotation(-90.f);
        drawNode->setPosition(circle->getPosition());

        auto clippingNode = CCClippingNode::create(drawNode);
        clippingNode->setTag(6);
        clippingNode->addChild(circle);
        clippingNode->setVisible(false);
        this->addChild(clippingNode);

        auto glabel = CCLabelTTF::create("Loading Geometry Dash...", "Segoe UI", 20.f);
        glabel->setPosition({ winSize.width / 2, 40.f });
        glabel->setOpacity(0);
        glabel->setScale(.7f);
        glabel->setTag(4);
        glabel->runAction(CCSequence::create(
            CCDelayTime::create(3.f),
            CCSpawn::create(
                CCEaseExponentialIn::create(
                    CCFadeTo::create(2.f, 255)
                ),
                CCEaseExponentialInOut::create(
                    CCMoveBy::create(2.f, { 0.f, 60.f })
                ),
                nullptr
            ),
            nullptr
        ));
        this->addChild(glabel);

        auto gloadlabel = CCLabelTTF::create("", "Segoe UI", 20.f);
        gloadlabel->setPosition({ winSize.width / 2, 10.f });
        gloadlabel->setOpacity(0);
        gloadlabel->setScale(.4f);
        gloadlabel->setTag(5);
        gloadlabel->runAction(CCSequence::create(
            CCDelayTime::create(3.5f),
            CCSpawn::create(
                CCEaseExponentialIn::create(
                    CCFadeTo::create(2.f, 150)
                ),
                CCEaseExponentialInOut::create(
                    CCMoveBy::create(2.f, { 0.f, 60.f })
                ),
                nullptr
            ),
            CCCallFunc::create(this, callfunc_selector(CustomLoadingLayer::actuallyLoad)),
            nullptr
        ));
        this->addChild(gloadlabel);

        // auto [count, unresolvedCount] = Loader::get()->getLoadedModCount();

        // const char* text = "test";
        // commented out until someone figures out why this crashes
        // const char* text = unresolvedCount ?
        //     CCString::createWithFormat("Geode: Loaded %zu mods (%zu unresolved)", count, unresolvedCount)->getCString() : 
        //     CCString::createWithFormat("Geode: Loaded %zu mods", count)->getCString();

        // auto label = CCLabelBMFont::create(text, "goldFont.fnt");
        // label->setPosition(winSize.width / 2, 30.f);
        // label->setScale(.45f);
        // label->setTag(5);
        // this->addChild(label);

        return true;
    }

    void actuallyLoad() {
        this->setUserData(as<void*>(1));
        this->loadAssets();
    }

    void loadingFinished() {
        getChild<CCNode*>(this->getChildByTag(2), 0)->runAction(CCSequence::create(
            CCDelayTime::create(.2f),
            CCSpawn::create(
                CCEaseBackIn::create(
                    CCScaleTo::create(1.2f, 0.f)
                ),
                CCEaseIn::create(
                    CCRotateBy::create(2.f, -30.f),
                    2.f
                ),
                CCEaseIn::create(
                    CCFadeTo::create(2.f, 0),
                    2.f
                ),
                nullptr
            ),
            nullptr
        ));
        getChild<CCNode*>(this->getChildByTag(3), 0)->runAction(CCSequence::create(
            CCSpawn::create(
                CCEaseBackIn::create(
                    CCScaleTo::create(1.5f, 0.f)
                ),
                CCEaseIn::create(
                    CCRotateBy::create(2.f, 30.f),
                    2.f
                ),
                CCEaseIn::create(
                    CCFadeTo::create(2.f, 0),
                    2.f
                ),
                nullptr
            ),
            nullptr
        ));
        this->getChildByTag(4)->runAction(CCSequence::create(
            CCEaseExponentialIn::create(
                CCFadeTo::create(1.f, 0)
            ),
            nullptr
        ));
        getChild<CCNode*>(as<CCClippingNode*>(this->getChildByTag(6)), 0)->runAction(CCSequence::create(
            CCEaseIn::create(
                CCFadeTo::create(1.f, 0),
                2.f
            ),
            CCCallFunc::create(this, callfunc_selector(CustomLoadingLayer::goToMenuLayer)),
            nullptr
        ));
        as<CCLabelTTF*>(this->getChildByTag(5))->runAction(CCSequence::create(
            CCDelayTime::create(.2f),
            CCEaseIn::create(
                CCFadeTo::create(.5f, 0),
                2.f
            ),
            nullptr
        ));
        this->getChildByTag(1)->runAction(CCFadeTo::create(2.f, 0));
    }

    void goToMenuLayer() {
        if (!this->m_fromRefresh) {
            GameManager::sharedState()->fadeInMusic("menuLoop.mp3");
        }
        CCDirector::sharedDirector()->replaceScene(
            CCTransitionFade::create(1.f,
                MenuLayer::scene(this->m_fromRefresh)
            )
        );
    }

	void loadAssets() {
        auto circleClip = as<CCClippingNode*>(this->getChildByTag(6));
        auto stencil = as<CCDrawNode*>(circleClip->getStencil());
        circleClip->setVisible(true);

        stencil->clear();
        drawSector(stencil, { 0, 0 }, 120.f, this->m_loadStep / 15.f * -360.f);

        if (this->m_loadStep == 0xe) {
            std::vector<CCPoint> points = {
                { -500.f, -300.f },
                { -500.f,  300.f },
                {  500.f,  300.f },
                {  500.f, -300.f },
            };
            stencil->drawPolygon(
                points.data(), points.size(),
                { 1.f, 1.f, 1.f, 1.f }, 0, { 0.f, 0.f, 0.f, 0.f }
            );
            this->m_sliderBar->release();
            return this->loadingFinished();
        }

        auto str = "Loading " + loadStepToString(this->m_loadStep + 1);
        as<CCLabelTTF*>(this->getChildByTag(5))->setString(str.c_str());

        if (this->getUserData()) {
		    $LoadingLayer::loadAssets();
        }
	}
};
