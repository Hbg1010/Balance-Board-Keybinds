#pragma once

#include <Geode/Geode.hpp>
#include "../BBKeybind.hpp"
#include "../customGUI/waitingLayer.hpp"
#include "../utils/BBDisconnectEvent.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
class $modify(playLayerCalibrate, PlayLayer) {
    struct Fields {
        bool pauseThis;
    };

    void createWaitingLayer(bool firstTime = false);
    
    // hooks
    void startGame();
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
};
