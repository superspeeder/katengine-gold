#include "kat/core.hpp"
#include <fstream>

namespace kat {
    rapidjson::Document loadJSON(const std::string& path) {
        std::string text;
        try {
            text = readFile(path);
        } catch (std::exception e) {
            spdlog::error("Failed to load JSON  from path `{}`. Using blank json.", path);
            text = "";
        }
        return loadJSONMemory(text);
    }

    rapidjson::Document loadJSONMemory(const std::string& text) {
        rapidjson::Document document;
        document.Parse(text.c_str());
        return document;
    }

    JsonSchema* loadJSONSchema(const std::string& path) {
        std::string text;
        try {
            text = readFile(path);
        } catch (std::exception e) {
            spdlog::error("Failed to load JSON schema from path `{}`. Using blank schema.", path);
            text = "{}";
        }
        return loadJSONSchemaMemory(text);
    }

    JsonSchema* loadJSONSchemaMemory(const std::string& text) {
        rapidjson::Document doc;
        doc.Parse(text.c_str());
        if (doc.HasParseError()) {
            spdlog::error("Schema JSON is invalid. Using blank schema.");
            doc.Parse("{}");
        }


        return new JsonSchema(doc);
    }

    std::string readFile(const std::string& path) {
        std::ifstream f(path, std::ios::ate);

        if (!f.good()) {
            throw std::runtime_error("Failed to read file `" + path + "`.");
        }

        size_t size = f.tellg();
        f.seekg(0);
        char* buf = new char[size];
        f.read(buf, size);
        f.close();

        return buf;
    }

    rapidjson::Document loadJSON(const std::string& path, JsonSchema* schema) {
        rapidjson::Document d = loadJSON(path);
        if (schema->isDocumentValid(d)) {
            return d;
        }

        throw std::runtime_error("Schema failed to validate");
    }

    rapidjson::Document loadJSONMemory(const std::string& text, JsonSchema* schema) {
        rapidjson::Document d = loadJSONMemory(text);
        if (schema->isDocumentValid(d)) {
            return d;
        }

        throw std::runtime_error("Schema failed to validate");
    }

    bool JsonSchema::isDocumentValid(rapidjson::Document& doc) {
        return doc.Accept(validator);
    }

    JsonSchema::JsonSchema(rapidjson::Document& doc_) : doc(doc_), validator(doc) {

    }


}