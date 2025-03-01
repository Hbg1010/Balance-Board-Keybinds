// #include "menu.hpp"

// bool menu::init() {
//     if (!MenuLayer::init()) return false;
//         wiimote* extra_motes [7] = { NULL }; // 7 should cover it
//         wiimote *next = new wiimote;
//         if(!next->Connect(wiimote::FIRST_AVAILABLE)) {
//             FLAlertLayer::create(
//                 "Title",    // title
//                 "Hi mom!",  // content
//                 "OK"        // button
//             )->show();
//         }
//         return true;
// }