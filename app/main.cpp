#include "main.h"
#include "kat/Engine.h"
#include "TestApp.h"

#include <spdlog/spdlog.h>

int main() {
    spdlog::info("Hello World!");

    kat::Engine engine;
    engine.launchApp<TestApp>();

    return 0;
}