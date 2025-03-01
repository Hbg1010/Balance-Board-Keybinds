#pragma once
#include "../include/wiimote.h"

class BalanceBoard {
protected:
    BalanceBoard(wiimote* input);
    ~BalanceBoard();
    static wiimote* balanceBoard;
    static float min;
    //  BalanceBoard* obj;

public:
    static BalanceBoard* create(wiimote* input);
    static float getWeight();
    static void setMin(float x);
    static bool onScale();
    static bool connected();
    static bool APressed() const;
};