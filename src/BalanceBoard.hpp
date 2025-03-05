#pragma once
#include "../include/wiiuse/wiiuse.h"
#include "BBKeybind.hpp"
#include "utils/weightUtils.hpp"

using namespace keybinds;

class BalanceBoard {
protected:
    BalanceBoard(wii_board_t* input);
    ~BalanceBoard();
    static wiimote** bbWii;
    static wii_board_t* balanceBoard;

    static float min;
    static bool wasOnScale;
    //  BalanceBoard* obj;

public:
    static BalanceBoard* create(wiimote* input);
    static float getWeight();
    static void setMin(float x);
    static bool onScale();
    static bool connected();
    // static bool APressed();
    static void disconnect();
    static bool checkAndTryConnect();
    static bool FORCEDISCONNECT;

    static void balanceBoardCheckLoop();
};