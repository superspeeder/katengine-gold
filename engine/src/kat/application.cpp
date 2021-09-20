#include "kat/application.hpp"
#include <rapidjson/document.h>
#include <fstream>


namespace kat {
    Application::Application() {

    }

    Application::~Application() {

    }

    void Application::launch() {
        try {
            load();
            createInternal();
            create();
            postCreate();
            mainloop();
            preCleanup();
            cleanup();
            cleanupInternal();
        } catch (std::exception& e) {
            exit(crash(e));
        }
    }

    void Application::load() {
        // load configs

        m_Config = ApplicationConfig::load("appcfg.json");
    }

    void Application::createInternal() {
        // create internal systems (window, context, etc)
    }

    void Application::postCreate() {
        // create requested/required systems
    }

    void Application::updateEvents() {
        // poll and start event handling
    }

    void Application::updateInternal(double dt) {
        // update data to prepare for the user update function
    }
    
    void Application::renderInternal() {
        // render all the things
    }

    void Application::mainloop() {
        // run the game mainloop
        
        // clock setups
        std::chrono::high_resolution_clock clock;
        std::chrono::time_point now = clock.now();
        std::chrono::time_point lastFrame = now - SIXTIETH_SECOND; // simulate as last frame at 60fps
        std::chrono::duration<double> dt = now - lastFrame;

        while (m_Running) {
            // event system
            updateEvents();
            
            // updates
            updateInternal(dt.count());
            update(dt.count());
            
            // render
            renderInternal();
            
            // update frame times
            lastFrame = now;
            now = clock.now();
            dt = now - lastFrame;
        }
    }

    void Application::preCleanup() {
        // prepare application to be cleaned up. runs vkDeviceWaitIdle when using the vulkan backend
    }

    void Application::cleanupInternal() {
        // cleanup internal systems
    }

    int Application::crash(std::exception& e) {
        // log error
        spdlog::critical("Well Fuck. Application crashed.");
        spdlog::critical("Error Message: {}", e.what());
        return 1;
    }

    void Application::close() {
        m_Running = false;
    }

    ApplicationConfig ApplicationConfig::load(const std::string& path) {
        // read file



//        rapidjson::Document cfgdoc = loadJSON(path, );
        
        return {};
    }
}