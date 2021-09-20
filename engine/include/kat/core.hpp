#pragma once

// prevent c header warnings since cpr uses deprecated c headers AAAA
#define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
#include <optional>
#include <chrono>
#include <string>
#include <concepts>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include <cpr/cpr.h>

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>

#define CREATE_EXCEPTION(name) class name : public std::exception { private: std::string errormsg; public: inline name(const std::string& msg) { errormsg = #name ": "; errormsg = errormsg + msg;  }; inline const char* what() const { return errormsg.c_str(); }; }

namespace kat {
    const auto SIXTIETH_SECOND = std::chrono::duration<double>(1.0 / 60.0);

    rapidjson::Document loadJSON(const std::string& path);
    rapidjson::Document loadJSONMemory(const std::string& text);


    struct JsonSchema {
        JsonSchema(rapidjson::Document& doc);

        rapidjson::SchemaDocument doc;
        rapidjson::SchemaValidator validator;

        bool isDocumentValid(rapidjson::Document& doc);
    };

    CREATE_EXCEPTION(json_schema_invalidation);

    /**
     * @brief 
     * 
     * @param path the path to the json
     * @param schema the schema to validate against
     * @return an optional that contains the json document if it passes validatin 
     */
    rapidjson::Document loadJSON(const std::string& path, JsonSchema* schema);
    rapidjson::Document loadJSONMemory(const std::string& text, JsonSchema* schema);

    JsonSchema* loadJSONSchema(const std::string& path);
    JsonSchema* loadJSONSchemaMemory(const std::string& text);

    std::string readFile(const std::string& path);

    template<typename Container>
    class unordered_inserter {
    public:
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using reference_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using container_type = Container;

        inline unordered_inserter& operator++() {return *this;} //no-op
        inline unordered_inserter& operator++(int) {return *this;} //no-op
        inline unordered_inserter& operator*() {return *this;} //no-op
        inline constexpr unordered_inserter& operator=(const typename Container::value_type& value) {
            container->insert(value);
            return *this;
        }
        inline constexpr unordered_inserter& operator=(typename Container::value_type&& value) {
            container->insert(std::move(value));
            return *this;
        }
        inline unordered_inserter(Container* container)
            :    container(container)
        {}
    protected:
        Container* container;
    };
}