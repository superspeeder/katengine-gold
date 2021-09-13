#include "sandbox/main.hpp"

SandboxApp::SandboxApp(const kat::ApplicationConfig& cfg) : kat::Application(cfg) {

}

SandboxApp::~SandboxApp() {

}

void SandboxApp::create() {
    spdlog::info("Create");
}

void SandboxApp::update(double dt) {
    if (++count >= 10e6) {
        close();
    }
}

void SandboxApp::cleanup() {
    spdlog::info("Cleanup");
}

int main() {
    kat::ApplicationConfig cfg{};
    SandboxApp app(cfg);

    app.launch();
    return 0;
}