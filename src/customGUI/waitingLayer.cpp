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

    this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
        if (event->isDown()) {
            this->onClose(nullptr);
        }
        // Return Propagate if you want other actions with the same bind to
        // also be fired, or Stop if you want to halt propagation
        return ListenerResult::Propagate;
    }, "backflip"_spr);

    return true;
}