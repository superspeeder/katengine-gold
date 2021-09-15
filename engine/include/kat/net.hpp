#pragma once

#include "kat/core.hpp"


namespace kat {
    JsonSchema loadJSONSchemaURL(const std::string& url);
    rapidjson::Document loadJSONURL(const std::string& url);
    rapidjson::Document loadJSONURL(const std::string& url, JsonSchema& schema);

    namespace net {
        std::string get(const std::string& url);
    }
}