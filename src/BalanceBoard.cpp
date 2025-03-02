#include "BalanceBoard.hpp"

wiimote* BalanceBoard::balanceBoard = nullptr; // Or initialize properly
float BalanceBoard::min = 20;
bool BalanceBoard::wasOnScale = false;
bool BalanceBoard::FORCEDISCONNECT = false;

BalanceBoard::BalanceBoard(wiimote* input) {
    BalanceBoard::min = 20;
    BalanceBoard::balanceBoard = input;
}

BalanceBoard::~BalanceBoard() {
    BalanceBoard::balanceBoard->Disconnect();
    delete BalanceBoard::balanceBoard;
}

BalanceBoard* BalanceBoard::create(wiimote* input) {
    if (input != nullptr && input->IsBalanceBoard()) {

        BalanceBoard* temp = new BalanceBoard(input);
        return temp;
    } else {
        return nullptr;
    }
}

float BalanceBoard::getWeight() {
    if (BalanceBoard::balanceBoard->IsConnected()) {
        return BalanceBoard::balanceBoard->BalanceBoard.Lb.Total;
    } else {
        return -1;
        // throw event
    }
}

void BalanceBoard::setMin(float x) {
    if (x > 0) BalanceBoard::min = x;
}

bool BalanceBoard::onScale() {
    if (BalanceBoard::balanceBoard->IsConnected()) {
        return BalanceBoard::balanceBoard->BalanceBoard.Lb.Total > BalanceBoard::min;
    } else {
        // throw event
        return false;
    }
}

bool BalanceBoard::connected() {
    return std::ref(BalanceBoard::balanceBoard) != nullptr && BalanceBoard::balanceBoard->IsConnected();
}

bool BalanceBoard::checkAndTryConnect() {
    if (connected) return true;

    // TODO: ASSUMES ONLY 1 CONNECTED
    if (balanceBoard->Connect(wiimote::FIRST_AVAILABLE) && balanceBoard->IsBalanceBoard()){
        return true;
    } else {
        return false;
    }
}

bool BalanceBoard::APressed() {
    if (balanceBoard != nullptr) {
        return balanceBoard->Button.A();
    } else {
        return false;
    }
}

void BalanceBoard::disconnect() {
    BalanceBoard::balanceBoard->Disconnect();
}

// This loop will run on a seperate thread. there it will constantly check balance board state
void BalanceBoard::balanceBoardCheckLoop(){
    while(BalanceBoard::balanceBoard->IsConnected() && !FORCEDISCONNECT){
        while(balanceBoard->RefreshState() == NO_CHANGE) Sleep(1);

        if (wasOnScale != onScale()) {
            wasOnScale = !wasOnScale;

            if (wasOnScale) {
                geode::Loader::get()->queueInMainThread([=] {
                    PressBindEvent(BBKeybind::create(getWeight(), onScale()), true).post();
                }); 
            } else {
                geode::Loader::get()->queueInMainThread([=] {
                    PressBindEvent(BBKeybind::create(getWeight(), onScale()), false).post();
                }); 
            }
            
        }
    }

    geode::Loader::get()->queueInMainThread([] {
        geode::log::debug("lost connection!");
    });
}