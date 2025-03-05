#include "BalanceBoard.hpp"

wiimote** BalanceBoard::bbWii = nullptr;
wii_board_t* BalanceBoard::balanceBoard = nullptr; // Or initialize properly
float BalanceBoard::min = 20;
bool BalanceBoard::wasOnScale = false;
bool BalanceBoard::FORCEDISCONNECT = false;

BalanceBoard::BalanceBoard(wii_board_t* input) {
    BalanceBoard::min = 20;
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
    if (WIIMOTE_IS_CONNECTED(bbWii[0])) {
        float total = balanceBoard->tl + balanceBoard->tr + balanceBoard->bl + balanceBoard->br;
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
        return balanceBoard->tl + balanceBoard->tr + balanceBoard->bl + balanceBoard->br > BalanceBoard::min;
    } else {
        // throw event
        return false;
    }
}

bool BalanceBoard::connected() {
    return bbWii != nullptr && WIIMOTE_IS_CONNECTED(bbWii[0]);
}

bool BalanceBoard::checkAndTryConnect() {
    if (connected()) return true;

    wiimote** temp = nullptr;
    int c;
    // TODO: ASSUMES ONLY 1 CONNECTED
    if (!wiiuse_find(temp, 1, 1)) return false;
    
    c = wiiuse_connect(temp, 1);
    if (c && temp[0]->exp.type == EXP_WII_BOARD){
        geode::log::debug("x");
        bbWii = temp;
        balanceBoard = (wii_board_t*)bbWii[0];
        return true;
    } else {
        geode::log::debug("not a balance board!");
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
    geode::Loader::get()->queueInMainThread([] {
        geode::log::debug("lost connection!");
    });

    while(connected() && !FORCEDISCONNECT){
        if (wiiuse_poll(bbWii, 1)) {
            switch (bbWii[0]->event) {
                case WIIUSE_EVENT:
                    if (wasOnScale != onScale()) {
                        wasOnScale = !wasOnScale;

                        if (wasOnScale) {
                            geode::Loader::get()->queueInMainThread([=] {
                                PressBindEvent(BBKeybind::create(onScale()), false).post();
                            }); 
                        } else {
                            geode::Loader::get()->queueInMainThread([=] {
                                PressBindEvent(BBKeybind::create(onScale()), true).post();
                            }); 
                        }
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
                    // handle_ctrl_status(bbWii[0]);
                    // wiiuse_disconnected(bbWii[0]);
                    break;

            }
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
    }
    endLoop:
    // this happens when connect is lost
    geode::Loader::get()->queueInMainThread([] {
        PressBindEvent(BBKeybind::create(true), false).post();
        geode::log::debug("lost connection!");
    });
}