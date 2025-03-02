#include "WeightPopup.hpp"

CCLabelBMFont* weightTxt; // this is the timer element found on screen. it is updated to represent the timer!

WeightPopup* WeightPopup::create(float const& input) {
    auto temp = new WeightPopup();

    if (temp->initAnchored(360, 180, input)) {
        temp->autorelease();
        return temp;
    } else {
        CC_SAFE_DELETE(temp);
        return nullptr;
    }
}

bool WeightPopup::setup(float const& input) {
    this->setTitle("Weight");
    log::debug("{}", input);

    weightTxt = CCLabelBMFont::create(fmt::format("{} seconds", input).c_str(), "bigFont.fnt");
    weightTxt->setPosition(m_mainLayer->getContentWidth()/2,m_mainLayer->getContentHeight()/2);
    m_mainLayer->addChild(weightTxt);
    weightTxt->setID("Weight-Text"_spr);

    // set button
    CCMenu* confirmMenu = CCMenu::create();
    confirmMenu->setPosition({m_mainLayer->getContentWidth()/2, m_mainLayer->getScaledContentHeight()/8});
    m_mainLayer->addChild(confirmMenu);
    auto setSpr = ButtonSprite::create("Get");
    CCMenuItemSpriteExtra* setBtn = CCMenuItemSpriteExtra::create(setSpr, this, menu_selector(WeightPopup::onCheck));
    confirmMenu->addChild(setBtn);

    return true;
}

void WeightPopup::onCheck(CCObject* sender) {
    weightTxt->setString(fmt::format("{}", BalanceBoard::getWeight()).c_str());
}