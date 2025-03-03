#pragma once

#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class BBDisconnectEvent : public Event {
protected: 
    bool isDisabled;

public:
    BBDisconnectEvent(bool disabled);
    bool disabled() const;
};
