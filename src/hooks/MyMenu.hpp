#include <Geode/Geode.hpp>
// #include "../customGUI/WeightPopup.hpp"
#include "../customGUI/waitingLayer.hpp"
#include "../BalanceBoard.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace geode::prelude;

class $modify(MyMenu, MenuLayer) {
    bool init();
    void openWeight(CCObject* sender);  
};