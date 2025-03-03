#include "BBKeybind.hpp"

BBKeybind* BBKeybind::create(bool onB) {
    auto ret = new BBKeybind();
    ret->autorelease();
    // ret->weight = w;
    ret->onScale = onB;
    return ret;
}

bool BBKeybind::isEqual(Bind* other) {
    if (auto o = geode::cast::typeinfo_cast<BBKeybind*>(other)) {
        return weight == o->weight && onScale == o->onScale;
    }
    return false;
}

DeviceID BBKeybind::getDeviceID() const {
    return "balance_board"_spr;
}

matjson::Value BBKeybind::save() const {
    return matjson::makeObject({
        { "onBoard", static_cast<bool>(onScale)}
    });
}

BBKeybind* BBKeybind::parse(matjson::Value const& json) {
    return BBKeybind::create(
        // static_cast<float>(json["weight"].asDouble().unwrapOr(0)),
        static_cast<bool>(json["onBoard"].asBool().unwrapOr(false))
    );
}

// Get the hash for this bind
size_t BBKeybind::getHash() const {
    return std::hash<float>()(weight) ^ 0x8372; //static_cast<bool>(onScale);
}

bool BBKeybind::isOnBoard() const {
    return onScale;
}

float BBKeybind::getWeight() const {
    return weight;
}

std::string BBKeybind::toString() const {
    return onScale ? "On Board" : "On Board";
}
