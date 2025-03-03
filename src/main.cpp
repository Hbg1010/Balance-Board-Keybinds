#include <Geode/Geode.hpp>
#include "../include/wiimote.h"
#include <mmsystem.h>
#include "BalanceBoard.hpp"
#include "BBKeybind.hpp"
using namespace geode::prelude;

#define MAXBOARDS 1

#include <Geode/modify/PlayLayer.hpp>

// class $modify(PlayLayer) {

//     struct Fields {
//         bool pauseThis;
//         wiimote* mote;
//     };

    
//     bool init(GJGameLevel* level, bool useReplay, bool p2) {
//         if (!PlayLayer::init(level, useReplay, p2)) return false;
//         wiimote** remote;
//         wiimote* balanceBoard = nullptr;
//         for (unsigned int i = 0; i < 5; i++) {
//             wiimote *next = new wiimote;
//             if (next->Connect(wiimote::FIRST_AVAILABLE)) {
//                 if (!next->IsBalanceBoard()) {
//                     balanceBoard = next;
//                     break;
//                 } else {
//                     remote[i] = next;
//                 }
//             }
//         }

//         if (std::ref(balanceBoard) != nullptr) {
//             log::debug("x");
//             m_fields->mote = balanceBoard;
//         } else {
            
//         }
//         // auto alert = FLAlertLayer::create(
//         //     "Title",
//         //     "Hi mom!",
//         //     "OK"
//         // );
//         // alert->m_scene = this;
//         // alert->show();
//         // m_fields->pauseThis = true;
//         return true;
//     }

//     void resetLevel() {
//         if (m_fields->pauseThis) pauseGame(true);
//         if (m_fields->mote->Button.A()) {
//             log::debug("A");
//         }
//         PlayLayer::resetLevel();
//     }
// };

// $on_mod(Loaded) {
//     // BindManager::get()->attachDevice("balance_board"_spr, &BBKeybind::parse);
// }

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

    BindManager::get()->attachDevice("balance_board"_spr, &BBKeybind::parse);


    BindManager::get()->registerBindable({
        // ID, should be prefixed with mod ID
        "init"_spr,
        "Balance Board Init",
        "waits for the player to step on the balance board before starting the round!",
        // Default binds
        { BBKeybind::create(false), Keybind::create(KEY_Q, Modifier::None) },
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

