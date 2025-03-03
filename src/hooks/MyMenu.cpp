#include "MyMenu.hpp"

bool MyMenu::init() {
    if (!MenuLayer::init()) return false;
    auto weightTestSpr = CircleButtonSprite::createWithSpriteFrameName("gjHand_03_001.png");
    auto weightTestBtn = CCMenuItemSpriteExtra::create(weightTestSpr, this, menu_selector(MyMenu::openWeight));
    this->getChildByID("bottom-menu")->addChild(weightTestBtn);
    this->getChildByID("bottom-menu")->updateLayout();
    log::debug("x");

    this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
        if (event->isDown()) {
            log::debug("x");
        }
        // Return Propagate if you want other actions with the same bind to
        // also be fired, or Stop if you want to halt propagation
        return ListenerResult::Propagate;
    }, "backflip"_spr);

    return true;
}

void MyMenu::openWeight(CCObject* sender) {
    // auto x = WeightPopup::create(BalanceBoard::getWeight());
    auto x = waitingLayer::create();
    x->show();
} 