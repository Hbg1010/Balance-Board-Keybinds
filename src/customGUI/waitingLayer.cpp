#include "waitingLayer.hpp"

waitingLayer* waitingLayer::create(bool const& firstTime) {
    auto temp = new waitingLayer();

    if (temp->initAnchored(360, 180, firstTime)) {
        temp->autorelease();
        return temp;
    } else {
        CC_SAFE_DELETE(temp);
        return nullptr;
    }
}

bool waitingLayer::setup(bool const& firstTime) {
    this->setTitle(firstTime ? "Waiting For Balance Board" : "Connection Lost!");
    this->m_closeBtn->getParent()->removeChild(this->m_closeBtn);
    auto description = CCLabelBMFont::create(firstTime ? "Step on the Balance Board!" : "Reconnect the Balance Board", "bigFont.fnt");
    description->setScale(0.6f);
    description->setPosition(m_mainLayer->getContentWidth()/2,m_mainLayer->getContentHeight()/2);

    this->m_mainLayer->addChild(description);
    description->setID("Description-text"_spr);

    // maybe this is a safe way to do this :terror:
    this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
        if (event->isDown() || typeinfo_cast<BBKeybind*>(event)->isOnBoard()) {
            if (auto pl = PlayLayer::get()) {
                if (pl->m_isPaused) {
                    auto parent = pl->getParent();
                    if (auto pLayer = static_cast<PauseLayer*>(parent->getChildByType<PauseLayer*>(0))) {
                        pLayer->onResume(nullptr);
                    }
                }
            }

            this->onClose(nullptr);
        }
        // Return Propagate if you want other actions with the same bind to
        // also be fired, or Stop if you want to halt propagation
        return ListenerResult::Propagate;
    }, "init"_spr);

    return true;
}