#include "waitingLayer.hpp"

waitingLayer* waitingLayer::create() {
    auto temp = new waitingLayer();

    if (temp->initAnchored(360, 180)) {
        temp->autorelease();
        return temp;
    } else {
        CC_SAFE_DELETE(temp);
        return nullptr;
    }
}

bool waitingLayer::setup() {
    this->setTitle("Waiting For Balance Board");

    auto description = CCLabelBMFont::create("Step on the Balance Board!", "bigFont.fnt");
    description->setScale(0.6f);
    description->setPosition(m_mainLayer->getContentWidth()/2,m_mainLayer->getContentHeight()/2);

    this->m_mainLayer->addChild(description);
    description->setID("Description-text"_spr);

    // maybe this is a safe way to do this :terror:
    this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
        if (event->isDown()) {
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