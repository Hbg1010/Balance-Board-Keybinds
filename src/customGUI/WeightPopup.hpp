#pragma once

#include "../utils/weightUtils.hpp"
#include "../BalanceBoard.hpp"
using namespace geode::prelude;


class WeightPopup final : public Popup<float const&> {
public:
    static WeightPopup* create(float const& input);
protected:
    CCLabelBMFont* weightTxt;
    bool useLbs;
    bool setup(float const& input);
    void onCheck(CCObject* sender);
};

