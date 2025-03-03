#include <Geode/Geode.hpp>
#include "../BBKeybind.hpp"
#include "../customGUI/waitingLayer.hpp"
using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
class $modify(playLayerCalibrate, PlayLayer) {
    struct Fields {
        bool pauseThis;
    };

    void createWaitingLayer();
    
    // hooks
    void startGame();
};
