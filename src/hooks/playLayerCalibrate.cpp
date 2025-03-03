#include "playLayerCalibrate.hpp"

void playLayerCalibrate::startGame() {
    PlayLayer::startGame();
    createWaitingLayer();
}

void playLayerCalibrate::createWaitingLayer() {
    PlayLayer::pauseGame(true);
    auto x = waitingLayer::create();
    x->show();
}