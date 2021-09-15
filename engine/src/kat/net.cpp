#include "kat/net.hpp"

#include <cpr/cpr.h>

namespace kat {
    JsonSchema loadJSONSchemaURL(const std::string& url) {
        std::string text;
        try {
            text = net::get(url);
        } catch (std::runtime_error& e) {
            spdlog::error("Failed to load JSON schema from url `{}`. Using blank schema.", url);
            text = "{}";
        }
        return loadJSONSchemaMemory(text);
    }

    rapidjson::Document loadJSONURL(const std::string& url) {
        std::string text;
        try {
            text = net::get(url);
        } catch (std::runtime_error& e) {
            spdlog::error("Failed to load JSON from url. Using blank json", url);
            text = "";
        }
        return loadJSONMemory(text);
    }

    rapidjson::Document loadJSONURL(const std::string& url, JsonSchema& schema) {
        rapidjson::Document d = loadJSONURL(url);
        if (schema.isDocumentValid(d)) {
            return d;
        }

        throw std::runtime_error("Schema failed to validate");
    }

    namespace net {
        std::string get(const std::string& url) {
            cpr::Response r = cpr::Get(cpr::Url{url});
            if (r.status_code == 200) { // success!
                return r.text;
            }

            throw std:runtime_error("Failed to get `" + url + "`: " + std::to_string(r.status_code) + ": " r.reason);
        }
    }
}