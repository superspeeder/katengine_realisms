#pragma once

#include <kat/Engine.h>

class TestApp : public kat::App {
public:

    TestApp();
    virtual ~TestApp();

private:
    void setup() override;

    void update(double dt) override;

    void cleanup() override;

private:

};



