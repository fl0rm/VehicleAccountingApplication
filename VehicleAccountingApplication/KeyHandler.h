#pragma once

#include <windows.h>
#include <unordered_map>

class KeyHandler {
private:
    std::unordered_map<int, bool> keyState;

public:
    bool isKeyPressedOnce(int);
};