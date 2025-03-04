#include "WeightPopup.hpp"

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
    useLbs = Mod::get()->getSettingValue<bool>("lbs");

    std::string theString;
    if (input < 0) {
        theString = "Not connected!";
    } else {
        theString = fmt::format("{} {}", useLbs ? input : weightUtils::lbsToKg(input), useLbs ? "lbs" : "kg").c_str();
    }

    weightTxt = CCLabelBMFont::create(theString.c_str(), "bigFont.fnt");
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
    float weight = BalanceBoard::getWeight();
    std::string theString;

    if (weight < 0) {
        theString = "Not connected!";
    } else {
        theString = fmt::format("{} {}", useLbs ? weight : weightUtils::lbsToKg(weight), useLbs ? "lbs" : "kg");
    }
    weightTxt->setString(theString.c_str());
}