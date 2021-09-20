#pragma once

#include "kat/context.hpp"

namespace kat {


    struct ClearValues {
        glm::vec4 color {0.0f,0.0f,0.0f,1.0f};
        float depth = 0.0f;
        int stencil = 0;
    };

    class RenderStage {
    public:

        virtual bool shouldClearBeforeDraw() = 0;
        virtual ClearValues getClearValues() = 0;

    private:

    };

    class DrawStage : public RenderStage {

    };

    class PostProcessStage : public RenderStage {

    };


    class Renderer {
    
    };
}