#pragma once
#include "../BBKeybind.hpp"

using namespace geode::prelude;

class waitingLayer : public Popup<bool const&> {
protected:
    bool setup(bool const& input);
public:
    static waitingLayer* create(bool const& input);
};