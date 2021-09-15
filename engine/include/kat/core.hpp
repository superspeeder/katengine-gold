#pragma once

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <chrono>
#include <string>
#include <optional>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>


namespace kat {
    const auto SIXTIETH_SECOND = std::chrono::duration<double>(1.0 / 60.0);

    rapidjson::Document loadJSON(const std::string& path);
    rapidjson::Document loadJSONMemory(const std::string& text);


    struct JsonSchema {
        rapidjson::SchemaDocument doc;
        rapidjson::SchemaValidator validator;

        bool isDocumentValid(rapidjson::Document& doc);
    };


    /**
     * @brief 
     * 
     * @param path the path to the json
     * @param schema the schema to validate against
     * @return an optional that contains the json document if it passes validatin 
     */
    rapidjson::Document loadJSON(const std::string& path, JsonSchema& schema);
    rapidjson::Document loadJSONMemory(const std::string& text, JsonSchema& schema);

    JsonSchema loadJSONSchema(const std::string& path);
    JsonSchema loadJSONSchemaMemory(const std::string& text);

    std::string readFile(const std::string& path);
}