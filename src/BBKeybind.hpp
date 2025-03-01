#pragma once
// #include <Geode/Geode.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
// #include "BalanceBoard.hpp"

using namespace keybinds;

class BBKeybind : public Bind {
public:
    float weight;
    bool onScale;

    static BBKeybind* create(float weight, bool onBoard);
    bool isOnBoard() const;
    float getWeight() const;
    bool isEqual(Bind* other);
    size_t getHash() const override;

    static BBKeybind* parse(matjson::Value const& json);
    matjson::Value save() const override;
    std::string toString() const override;

    // The device this bind is related to
    DeviceID getDeviceID() const override;
};
