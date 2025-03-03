#include "MyMenu.hpp"

bool MyMenu::init() {
    if (!MenuLayer::init()) return false;
    auto weightTestSpr = CircleButtonSprite::createWithSpriteFrameName("gjHand_03_001.png");
    auto weightTestBtn = CCMenuItemSpriteExtra::create(weightTestSpr, this, menu_selector(MyMenu::openWeight));
    this->getChildByID("bottom-menu")->addChild(weightTestBtn);
    this->getChildByID("bottom-menu")->updateLayout();
    log::debug("x");

    return true;
}

void MyMenu::openWeight(CCObject* sender) {
    auto x = WeightPopup::create(BalanceBoard::getWeight());
    x->show();
} 