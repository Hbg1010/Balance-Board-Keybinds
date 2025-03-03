#pragma once
#include <Geode/Geode.hpp>
#include "../BBKeybind.hpp"

using namespace geode::prelude;

class waitingLayer : public Popup<> {
protected:
    bool setup();
public:
    static waitingLayer* create();
};