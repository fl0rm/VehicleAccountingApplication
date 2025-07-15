
#include "KeyHandler.h"

bool KeyHandler::isKeyPressedOnce(int vKey) {
    
    bool isPressedNow = (GetAsyncKeyState(vKey) & 0x8000) != 0;
    bool wasPressed = keyState[vKey];

    bool pressedOnce = isPressedNow && !wasPressed;
    keyState[vKey] = isPressedNow;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    if ((vKey == VK_RETURN) && pressedOnce) {
        std::cin.get();
    }
    return pressedOnce;
}