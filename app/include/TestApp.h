#pragma once

#include <kat/Engine.h>
#include <kat/Renderer.h>

class TestApp : public kat::App {
public:

    TestApp();
    virtual ~TestApp();

private:
    void setup() override;

    void update(double dt) override;

    void cleanup() override;

private:

    std::shared_ptr<kat::Renderer> m_Renderer;

};



