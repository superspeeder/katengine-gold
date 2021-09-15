#include "sandbox/main.hpp"

SandboxApp::SandboxApp() : kat::Application() {

}

SandboxApp::~SandboxApp() {

}

void SandboxApp::create() {
    spdlog::info("Create");
}

void SandboxApp::update(double dt) {

}

void SandboxApp::cleanup() {
    spdlog::info("Cleanup");
}

int main() {
    SandboxApp app;

    app.launch();
    return 0;
}