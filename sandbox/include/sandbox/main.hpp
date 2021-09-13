#pragma once

#include <kat/application.hpp>


class SandboxApp : public kat::Application {
public:
    SandboxApp(const kat::ApplicationConfig& cfg);
    ~SandboxApp();

    void create() override;
    void update(double dt) override;
    void cleanup() override;

    int count = 0;
};