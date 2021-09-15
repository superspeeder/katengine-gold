#pragma once

#include <kat/application.hpp>


class SandboxApp : public kat::Application {
public:
    SandboxApp();
    ~SandboxApp();

    void create() override;
    void update(double dt) override;
    void cleanup() override;

    int count = 0;
};