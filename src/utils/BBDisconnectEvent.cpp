#include "BBDisconnectEvent.hpp"

BBDisconnectEvent::BBDisconnectEvent(bool disabled) {
    isDisabled = disabled;
}

bool BBDisconnectEvent::disabled() const {
    return isDisabled;
}