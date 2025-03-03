#pragma once

#include <Geode/Geode.hpp>
#include "../utils/weightUtils.hpp"
#include "../BalanceBoard.hpp"
using namespace geode::prelude;


class WeightPopup final : public geode::Popup<float const&> {
public:
    static WeightPopup* create(float const& input);
protected:
    bool useLbs;
    bool setup(float const& input);
    void onCheck(CCObject* sender);
};

