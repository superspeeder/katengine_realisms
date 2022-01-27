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
    m_Renderer = std::make_shared<kat::Renderer>(m_Engine->getRunningApp());
}

void TestApp::update(double dt) {
    m_Renderer->render();
}

void TestApp::cleanup() {
    m_Renderer->cleanup();

    spdlog::info("Ran for {} frames, {} seconds; Average FPS: {}", m_Clock.getFrameCount(), m_Clock.getUptime().count(), m_Clock.getAverageFramesPerSecond());
}


