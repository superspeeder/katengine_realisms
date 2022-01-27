#include <spdlog/spdlog.h>
#include "TestApp.h"

TestApp::TestApp() : kat::App() {
    m_Configuration.window_mode = kat::WindowedWindowMode{
        .size = {800, 800},
        .resizable = false,
        .floating = false
    };
}

TestApp::~TestApp() {
}

void TestApp::setup() {

}

void TestApp::update(double dt) {
}

void TestApp::cleanup() {
    spdlog::info("Ran for {} frames, {} seconds; Average FPS: {}", m_Clock.getFrameCount(), m_Clock.getUptime().count(), m_Clock.getAverageFramesPerSecond());
}


