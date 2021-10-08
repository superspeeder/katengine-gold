#pragma once

#include "kat/renderer.hpp"

namespace kat {
    namespace vk {
        class ImageView {
        public:
        private:

            ::vk::ImageView m_ImageView;
        };

        class CommandBuffer {
        public:
        private:

            ::vk::CommandBuffer m_CommandBuffer
        };

        class CommandPool {
        public:
        private:

            ::vk::CommandPool m_CommandPool;
        };

        class Surface {
        public:
        private:

            ::vk::SurfaceKHR m_Surface;
        };

        class Swapchain {
        public:
        private:
            ::vk::SwapchainKHR m_Swapchain;
        };

        class Queue {
        public:
        private:
            ::vk::Queue m_Queue;
        };

        class Semaphore {
        public:
        private:
            ::vk::Semaphore m_Semaphore;
        };

        class Fence {
        public:
        private:
            ::vk::Fence m_Fence;
        };

        class Event {
        public:
        private:
            ::vk::Event m_Event;
        };

        class MemoryBarrier {
        public:
        private:
            ::vk::MemoryBarrier m_MemoryBarrier;
        };

        class RenderPass {
        public:
        private:
            ::vk::RenderPass m_RenderPass;
        };

        class Framebuffer : public kat::Framebuffer {
        public:
        private:
            ::vk::Framebuffer m_Framebuffer;
        };

        class Image : public kat::Image {
        public:
        private:
        };

        class Texture : public kat::Texture {
        public:
        private:
            
        };

        class TextureSampler : public kat::TextureSampler {
        public:
        private:
            
        };

        class ImageCompositor : public kat::ImageCompositor {
        public:
        private:

        };

        class RenderStage : public kat::RenderStage {
        public:
        private:

        };


        class Renderer : public kat::Renderer {
        public:
        private:

        };

        class Shader : public kat::Shader {
        public:
        private:

        };

        class ShaderPipeline : public kat::ShaderPipeline {
        public:
        private:

        };

        class RenderPipeline : public kat::RenderPipeline {
        public:
        private:

        };

        class ComputePipeline : public kat::ComputePipeline {
        public:
        private:

        };
        
        class PipelineCache {
        public:
        private:
        
        };

        class PipelineLibrary {
        public:
        private:

        };

        class HostMemory {
        public:
        private:

        };

        class DeviceMemory {
        public:
        private:

        };


        class Buffer {
        public:
        private:

        };

        class BufferView {
        public:
        private:

        };

        class DescriptorSetLayout {
        public:
        private:

        };

        enum class DescriptorType {

        };

        class PipelineLayout {
        public:
        private:

        };

        class DescriptorPool {
        public:
        private:

        };

        class DescriptorSet {
        public:
        private:

        };

        class QueryPool {
        public:
        private:

        };
    }
}