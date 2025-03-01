#include "BalanceBoard.hpp"

wiimote* BalanceBoard::balanceBoard = nullptr; // Or initialize properly
float BalanceBoard::min = 20;

BalanceBoard::BalanceBoard(wiimote* input) {
    BalanceBoard::min = 20;
    BalanceBoard::balanceBoard = input;
}

BalanceBoard::~BalanceBoard() {
    delete BalanceBoard::balanceBoard;
}

BalanceBoard* BalanceBoard::create(wiimote* input) {
    if (input != nullptr) {
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

bool BalanceBoard::APressed() {
    if (balanceBoard != nullptr) {
        return balanceBoard->Button.A();
    }
}