#pragma once

#include "kat/context.hpp"

namespace kat {


    enum class ImageFormat {
        eR, eRG, eRGB, eRGBA, eDepthStencil
    }; // basic formats, may not mean the same on all platforms

    enum class MagnificationFilter {
        eNearest, eLinear
    };

    class Image { // image data

    };

    class Framebuffer {

    };

    class Texture { // vkimage will contain an imageview, glimage will contain an image only.

    };

    class TextureSampler {

    };
    
    /**
     * @brief Basic image composition class.
     * This class should be able to draw a bunch of images. this is most optimal with the vulkan backend, and multiple layers do not cost extra crosses of the bridge.
     * 
     * multiple draws will be run if there are more textures to composite than there are texture units (places to bind samplers)
     * On the OpenGL backend, this will generate mutliple draw calls and context switches, and on the vulkan backend all the logic will be packed into the command buffer. for vulkan a "baked" geometry will be used. Use the MultiTextureQuadBatch for dynamic quads with individual textures. these will use instanced rendering to draw many quads with different textures using the same draw call. draw calls will be split by the max texture units.
     * 
     * composition logic:
     * draw n textured quads which are ordered with their z index and are textured with the right texture out to the output texture.
     * 
     * 
     * 
     * logic for large number of textures being composited
     * 
     * composite incrementally to the output framebuffer (1 draw call per max set of textures)
     */
    class ImageCompositor {
    public:
        ImageCompositor(const ImageFormat& imgFmt);

        void setOutputFramebuffer(Framebuffer* fbo);

        void insertImage(TextureSampler* tex, int z);
        void pushBackImage(TextureSampler* tex);
        void pushFrontImage(TextureSampler* tex);
        void removeImage(TextureSampler* tex);

        void composite(); // should run the draw operation. in the case of the vulkan backend this will execute the command buffer, and update it if necessary.

    private:

        int minZ = 0, maxZ = 0;
        Framebuffer* outputFramebuffer;
    };

    struct ClearValues {
        glm::vec4 color {0.0f,0.0f,0.0f,1.0f};
        float depth = 0.0f;
        int stencil = 0;
    };

    class RenderStage {
    public:

        virtual bool shouldClearBeforeRender() = 0;
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