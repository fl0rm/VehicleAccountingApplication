#include <windows.h>
#include <iostream>
#include <string>
#include <crtdbg.h>

#include "GCLI.h"
#include "TableUnitVehicle.h"
#include "ExternalDependencies.h"

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    GCLI consol;

    consol.Start();

    return 0;
}
