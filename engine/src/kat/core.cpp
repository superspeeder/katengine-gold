#include "kat/core.hpp"
#include <fstream>

namespace kat {
    rapidjson::Document loadJSON(const std::string& path) {
        std::ifstream f(path);
        std::string contents((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

        rapidjson::Document document;
        document.Parse(contents.c_str());
        return document;
    }
}