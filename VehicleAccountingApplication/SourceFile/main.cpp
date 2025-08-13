#include <windows.h>
#include <crtdbg.h>

#include "Core/Launcher.h"

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    Core::Launcher* launcher = new Core::Launcher();

    return launcher ? launcher->exec() : 1;
}
