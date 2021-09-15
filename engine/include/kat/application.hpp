#pragma once

#include "kat/core.hpp"

namespace kat {

    struct ApplicationConfig {


        static ApplicationConfig load(const std::string& filename);
    };

    struct ApplicationMetrics {
// avg fps, uptime, other things, etc. when KAT_TRACE is defined, has things like malloc:free ratio, leak tracking info, etc.
    };

    class Application {
    public:

        Application();
        virtual ~Application();

        void launch();

        virtual void create() = 0;
        virtual void update(double dt) = 0;
        virtual void cleanup() = 0;

        void close();

    protected:
        [[nodiscard]] int crash(std::exception& e); // generally returns -1, but should try to gracefully exit based on circumstances of crash

    private:

        void updateEvents();
        void load();
        void createInternal();
        void mainloop();
        void updateInternal(double dt);
        void renderInternal();
        void cleanupInternal();
        void preCleanup();
        void postCreate();

        ApplicationConfig m_Config{};
        bool m_Running = true;
    };
}