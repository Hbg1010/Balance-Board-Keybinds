#include "playLayerCalibrate.hpp"

void playLayerCalibrate::startGame() {
    PlayLayer::startGame();
    createWaitingLayer(true);

}

void playLayerCalibrate::createWaitingLayer(bool firstTime) {
    PlayLayer::pauseGame(true);
    auto x = waitingLayer::create(firstTime);
    x->show();
}

bool playLayerCalibrate::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
    // this->template addEventListener<EventFilter<BBDisconnectEvent>()>([=](BBDisconnectEvent* ev) {
    //     createWaitingLayer();
    // });
    return true;
}

