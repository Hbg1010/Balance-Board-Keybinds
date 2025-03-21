#include "BalanceBoard.hpp"

wiimote** BalanceBoard::bbWii = nullptr;
wii_board_t* BalanceBoard::balanceBoard = nullptr; // Or initialize properly
float BalanceBoard::min = 30;
bool BalanceBoard::wasOnScale = false;
bool BalanceBoard::FORCEDISCONNECT = false;
bool BalanceBoard::done = true;

BalanceBoard::BalanceBoard(wii_board_t* input) {
    BalanceBoard::min = 0.000001;
    BalanceBoard::balanceBoard = input;
}

BalanceBoard::~BalanceBoard() {
    wiiuse_disconnected(bbWii[0]);
    delete bbWii;
    delete BalanceBoard::balanceBoard;
}

BalanceBoard* BalanceBoard::create(wiimote* input) {
    if (input != nullptr && WIIMOTE_IS_CONNECTED(input) && input->exp.type == EXP_WII_BOARD) {
        balanceBoard = (wii_board_t*) input;
        bbWii[0] = input;
        BalanceBoard* temp = new BalanceBoard(balanceBoard);
        return temp;
    } else {
        return nullptr;
    }
}

float BalanceBoard::getWeight() {
    if (connected()) {
        wii_board_t* x = (wii_board_t*) &bbWii[0]->exp.wb;
        float total = x->tl + x->tr + x->bl + x->tr;
        return total;
    } else {
        return -1.f;
        // throw event
    }
}

void BalanceBoard::setMin(float x) {
    if (x > 0) BalanceBoard::min = x;
}

bool BalanceBoard::onScale() {
    if (connected()) {
        return getWeight() > BalanceBoard::min;
    } else {
        // throw event
        return false;
    }
}

bool BalanceBoard::connected() {
    return bbWii != nullptr && WIIMOTE_IS_CONNECTED(bbWii[0]);
}

bool BalanceBoard::checkAndTryConnect() {
    geode::log::debug("starting");
    if (connected()) {
        BalanceBoard::done = true;
        setAtomicTrue();
        return true;
    }

    wiimote** temp = wiiuse_init(1);
    int c;
    // TODO: ASSUMES ONLY 1 CONNECTED
    if (!wiiuse_find(temp, 1, 1)) {
        BalanceBoard::done = false;
        setAtomicTrue();
        return false;
    }
    
    c = wiiuse_connect(temp, 1);

    if (!c) {
        setAtomicTrue();
        return true;
    }

    

    if (temp[0]->exp.type == EXP_WII_BOARD){
        geode::log::debug("x");
        bbWii = temp;
        wiiuse_poll(bbWii, 1);
        // balanceBoard = (wii_board_t*)&bbWii[0];
        setAtomicTrue();
        return true;
    } else {
        geode::log::debug("not a balance board!");
        setAtomicTrue();
        return false;
    }
}

// bool BalanceBoard::APressed() {
//     if (balanceBoard != nullptr) {
//         return balanceBoard->Button.A();
//     } else {
//         return false;
//     }
// }

void BalanceBoard::disconnect() {
    wiiuse_disconnected(bbWii[0]);
}

// This loop will run on a seperate thread. there it will constantly check balance board state
void BalanceBoard::balanceBoardCheckLoop(){
    while(connected() && !FORCEDISCONNECT){
        if (wiiuse_poll(bbWii, 1)) {
            // geode::log::debug("hello worldy");
            switch (bbWii[0]->event) {
                case WIIUSE_EVENT:
                    if (wasOnScale != onScale()) {
                        sendInput(wasOnScale); // we want to not jump when on the scale, and jump when on the scale
                        wasOnScale = !wasOnScale;
                    }
                    break;
                case WIIUSE_STATUS:
                    /* a status event occurred */
                    // handle_ctrl_status(bbWii[0]);
                    break;
                case WIIUSE_DISCONNECT:
                case WIIUSE_UNEXPECTED_DISCONNECT:
                case WIIUSE_MOTION_PLUS_REMOVED:
                    disconnect();
                    goto endLoop;
                    break;
            }
        }
    }

    endLoop:
    done = true;
    // this happens when connect is lost
    geode::Loader::get()->queueInMainThread([] {
        PressBindEvent(BBKeybind::create(true), false).post();
        geode::log::debug("lost connection!");
    });
}

void BalanceBoard::sendInput(bool val) {
    geode::Loader::get()->queueInMainThread([=] {
        PressBindEvent(BBKeybind::create(true), val).post();
    }); 
}

        // if (wasOnScale != onScale()) {
        //     wasOnScale = !wasOnScale;

        //     if (wasOnScale) {
        //         geode::Loader::get()->queueInMainThread([=] {
        //             PressBindEvent(BBKeybind::create(onScale()), false).post();
        //         }); 
        //     } else {
        //         geode::Loader::get()->queueInMainThread([=] {
        //             PressBindEvent(BBKeybind::create(onScale()), true).post();
        //         }); 
        //     }
        // }