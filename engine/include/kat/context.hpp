#pragma once

#include "kat/core.hpp"

namespace kat {


    class Context {
    public:

        Context();
        virtual ~Context();

    private:

    };

    class VulkanContext : public Context {
    public:
    
    private:

    };

    class OpenGLContext : public Context {
    public:
    
    private:

    };
}