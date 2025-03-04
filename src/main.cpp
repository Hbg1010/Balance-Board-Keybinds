#include "BalanceBoard.hpp"
using namespace geode::prelude;

#define MAXBOARDS 1

class BalanceBoardChecker : public CCObject{
protected:
    bool m_state = false;
    static std::thread m_BoardLoop; 
public:
    BalanceBoardChecker() {
        this->retain();
    }

    void checkBalanceBoard() {
        bool currentConnected = BalanceBoard::checkAndTryConnect();

        
        if (currentConnected != m_state) {
            m_state = currentConnected;

            if (m_state) {
                BindManager::get()->attachDevice("balance_board"_spr, &BBKeybind::parse);
                BalanceBoard::FORCEDISCONNECT = false;
                m_BoardLoop = std::thread(&BalanceBoard::balanceBoardCheckLoop);
                Notification::create(
                    "Balance Board Attached",
                    CCSprite::createWithSpriteFrameName("controllerBtn_A_001.png") // TODO: custom sprite
                )->show();
            } else {
                if (m_BoardLoop.joinable()) {
                    BalanceBoard::FORCEDISCONNECT = true;
                    log::debug("??!?!?!?");
                }
                BindManager::get()->detachDevice("balance_board"_spr);
                Notification::create(
                    "Balance Board Detached",
                    CCSprite::createWithSpriteFrameName("controllerBtn_B_001.png") // TODO: custom sprite
                )->show();
            }
        }
    }
};

std::thread BalanceBoardChecker::m_BoardLoop; 

$execute {

    // BindManager::get()->attachDevice("balance_board"_spr, &BBKeybind::parse);

    BindManager::get()->addBindTo("robtop.geometry-dash/jump-p1", BBKeybind::create(false));

    BindManager::get()->registerBindable({
        // ID, should be prefixed with mod ID
        "init"_spr,
        "Balance Board Init",
        "waits for the player to step on the balance board before starting the round!",
        // Default binds 
        { BBKeybind::create(true), Keybind::create(KEY_Q, Modifier::None) }, // TODO: REMOVE Q
        "BalanceBoard"
    });

	// check every second if a controller has been connected
	Loader::get()->queueInMainThread([] {
		CCScheduler::get()->scheduleSelector(
			schedule_selector(BalanceBoardChecker::checkBalanceBoard),
			new BalanceBoardChecker(), 1.f, false
		);
	});
}

