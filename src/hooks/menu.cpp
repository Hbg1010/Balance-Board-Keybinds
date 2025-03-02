// #include "menu.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include "../customGUI/WeightPopup.hpp"
#include "../BalanceBoard.hpp"

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto weightTestSpr = CircleButtonSprite::createWithSpriteFrameName("gjHand_03_001.png");
        auto weightTestBtn = CCMenuItemSpriteExtra::create(weightTestSpr, this, menu_selector(MyMenu::openWeight));
        this->getChildByID("bottom-menu")->addChild(weightTestBtn);
        this->getChildByID("bottom-menu")->updateLayout();
        log::debug("x");
        return true;
    }

    void openWeight(CCObject* sender) {
        auto x = WeightPopup::create(BalanceBoard::getWeight());
        x->show();
    } 
};
