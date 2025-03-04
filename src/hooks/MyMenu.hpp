#pragma once

#include "../customGUI/WeightPopup.hpp"
#include <Geode/modify/MenuLayer.hpp>
using namespace geode::prelude;

class $modify(MyMenu, MenuLayer) {
    bool init();
    void openWeight(CCObject* sender);  
};