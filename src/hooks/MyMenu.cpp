#include "MyMenu.hpp"

bool MyMenu::init() {
    if (!MenuLayer::init()) return false;

    auto weightTestSpr = CircleButtonSprite::createWithSprite("BalanceBoardIMG.png"_spr, 1.1f);
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