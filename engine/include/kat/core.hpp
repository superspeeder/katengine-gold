#pragma once

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <chrono>
#include <string>

#include <rapidjson/document.h>

namespace kat {
    const auto SIXTIETH_SECOND = std::chrono::duration<double>(1.0 / 60.0);

    rapidjson::Document loadJSON(const std::string& path);
}