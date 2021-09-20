#include "kat/assets.hpp"
#include <algorithm>

namespace kat {

    AssetCollection*& AssetCollection::operator[](const std::string& id) {
        return m_SubCollections[id];
    }

    void AssetCollection::push(const std::string& id, AssetCollection* collection) {
        m_SubCollections[id] = collection;
    }

    void AssetCollection::push(const AssetKey& key) {
        m_Assets.insert(key);
    }

    std::unordered_set<kat::AssetKey> AssetCollection::getAssets() {
        return m_Assets;
    }

    std::unordered_set<kat::AssetKey> AssetCollection::getAssetsOfType(const std::string_view& typeID) {
        std::unordered_set<kat::AssetKey> assets;
        std::copy_if(m_Assets.begin(), m_Assets.end(), unordered_inserter(&assets), [typeID](const kat::AssetKey& key) { return key.assetType == typeID; });
        return assets;
    }

    std::unordered_set<kat::AssetKey> AssetCollection::getAssetsWithID(const std::string& id) {
        std::unordered_set<kat::AssetKey> assets;
        std::copy_if(m_Assets.begin(), m_Assets.end(), unordered_inserter(&assets), [id](const kat::AssetKey& key) { return key.name == id; });
        return assets;
    }

    
    AssetLibrary::AssetLibrary() {
        // initialize asset library
    }


    AssetManager::AssetManager() {
        // initialize asset manager
    }

    Asset* AssetManager::getAsset(const std::string& typeID, const std::string& id) {
        return m_Library->getLibrary_base(typeID)->getAsset_base(id);
    }

    AssetCollection* AssetLibrary::getCollection(const std::string& id) {
        return m_Collections[id];
    }

    FileAsset* AssetManager::getFileAsset(const std::string& id) {
        return getAsset<FileAsset>(id);
    }


    Asset::~Asset() {
    }


    FileAsset::FileAsset(const std::string& filePath, bool binary) : m_File(filePath, binary? std::ios::binary | std::ios::in : std::ios::in) {
        open = m_File.good();
    }

    FileAsset::~FileAsset() {
    }

    std::vector<char> FileAsset::readCharArray() {
        std::vector<char> data((std::istreambuf_iterator<char>(m_File)),std::istreambuf_iterator<char>());
        return data;
    }

    std::string FileAsset::readString() {
        std::string data((std::istreambuf_iterator<char>(m_File)),std::istreambuf_iterator<char>());
        return data;
    }

    void FileAsset::dispose() {
        if (open) {
            m_File.close();
            open = false;
        }
    }

    bool FileAsset::available() {
        return open && m_File.good();
    }

    NetworkAsset::NetworkAsset(const std::string& url, bool downloadOnce) : m_URL{url}, m_CacheResponse{downloadOnce} {
        if (m_CacheResponse) {
            m_CachedResponse = cpr::GetAsync(m_URL);
        }
    }

    NetworkAsset::~NetworkAsset() {
    }

    std::vector<char> NetworkAsset::getTextCharArray() {
        std::string text = get().text;
        std::vector<char> arr(text.begin(), text.end());
        return arr;

    }

    std::string NetworkAsset::getTextString() {
        std::string text = get().text;
        return text;
    }

    cpr::Response NetworkAsset::get() {
        if (m_CacheResponse) {
            return m_CachedResponse.get();
        }
        return cpr::Get(m_URL);
    }

    void NetworkAsset::dispose() {
    }

    bool NetworkAsset::available() {
        if (m_CachedResponse.valid()) {
            return true;
        }
        else if (m_CachedResponse.get().status_code == 404) {
            return false;
        }
        return true;
    }

    DynamicSourcedAsset::DynamicSourcedAsset(const std::string& source, const AssetSource& src) {
        if (source.empty() && src != AssetSource::eMemory) {
            m_Available = false;
            spdlog::warn("Attempted to load dynamic asset from blank url or path");
            return;
        }


        switch (src) {
        case AssetSource::eURL:
            {
                auto resp = cpr::Get(cpr::Url{source});
                if (resp.status_code == 200) {
                    m_Content = resp.text;
                } else {
                    spdlog::warn("Failed to load asset from url `{0}` with status code {1}", resp.url.str(), resp.status_code);
                }
            }
            break;
        case AssetSource::eFile:
            {
                try {
                    m_Content = readFile(source);
                } catch (std::exception e) {
                    m_Available = false;
                    spdlog::error("Failed to load file `{}`", std::string(source));
                }
            }
            break;
        case AssetSource::eMemory:
            {
                m_Content = source;
            }
            break;
        }        
    }

    DynamicSourcedAsset::DynamicSourcedAsset(const char* source, const AssetSource& src) {
        if (source == nullptr && src != AssetSource::eMemory) {
            m_Available = false;
            spdlog::warn("Attempted to load dynamic asset from null url or path");
            return;
        }

        switch (src) {
        case AssetSource::eURL:
            {
                auto resp = cpr::Get(cpr::Url{source});
                if (resp.status_code == 200) {
                    m_Content = resp.text;
                } else {
                    m_Available = false;
                    spdlog::error("Failed to load asset from url `{0}` with status code {1}", resp.url.str(), resp.status_code);
                }
            }
            break;
        case AssetSource::eFile:
            {
                try {
                    m_Content = readFile(source);
                } catch (std::exception e) {
                    m_Available = false;
                    spdlog::error("Failed to load file `{}`", std::string(source));
                }
            }
            break;
        case AssetSource::eMemory:
            {
                m_Content = source;
            }
            break;
        }
    } 

    DynamicSourcedAsset::DynamicSourcedAsset(const std::vector<char>& source, const AssetSource& src) {
        std::string sourcestr;

        if (source.empty() && src != AssetSource::eMemory) {
            m_Available = false;
            spdlog::warn("Attempted to load dynamic asset from null url or path");
            return;
        }

        
        if (source[source.size() - 1] != 0) { // check if null terminated
            char* arr = reinterpret_cast<char*>(std::malloc(source.size() + 1));
            arr[source.size()] = 0;
            sourcestr = arr;
        } else {
            sourcestr = source.data();
        }
        
        switch (src) {
        case AssetSource::eURL:
            {
                auto resp = cpr::Get(cpr::Url{sourcestr});
                if (resp.status_code == 200) {
                    m_Content = resp.text;
                } else {
                    m_Available = false;
                    spdlog::error("Failed to load asset from url `{0}` with status code {1}", resp.url.str(), resp.status_code);
                }
            }
            break;
        case AssetSource::eFile:
            {
                try {
                    m_Content = readFile(sourcestr);
                } catch (std::exception e) {
                    m_Available = false;
                    spdlog::error("Failed to load file `{}`", sourcestr);
                }
            }
            break;
        case AssetSource::eMemory:
            {
                m_Content = sourcestr;
            }
            break;
        }
    }

    DynamicSourcedAsset::~DynamicSourcedAsset() {
    }

    std::vector<char> DynamicSourcedAsset::getTextCharArray() {
        return std::vector<char>(m_Content.begin(), m_Content.end());
    }

    std::string DynamicSourcedAsset::getTextString() {
        return m_Content;
    }

    void DynamicSourcedAsset::dispose() {
    }

    bool DynamicSourcedAsset::available() {
        return m_Available;
    }

    JsonSchemaAsset::JsonSchemaAsset(const std::string& source, const AssetSource& src) : DynamicSourcedAsset::DynamicSourcedAsset(source, src) {
        m_Schema = loadJSONSchemaMemory(m_Content);
    }

    JsonSchemaAsset::JsonSchemaAsset(const std::vector<char>& source, const AssetSource& src) : DynamicSourcedAsset::DynamicSourcedAsset(source, src) {
        m_Schema = loadJSONSchemaMemory(m_Content);
    }

    JsonSchemaAsset::JsonSchemaAsset(const char* source, const AssetSource& src) : DynamicSourcedAsset::DynamicSourcedAsset(source, src) {
        m_Schema = loadJSONSchemaMemory(m_Content);
    }

    JsonSchemaAsset::~JsonSchemaAsset() {
        disposeSchema();
        DynamicSourcedAsset::dispose();
    }

    void JsonSchemaAsset::dispose() {
        disposeSchema();
        DynamicSourcedAsset::dispose();
    }

    void JsonSchemaAsset::disposeSchema() {
        if (m_Schema != nullptr) {
            delete m_Schema;
            m_Schema = nullptr;
        }
    }

    bool JsonSchemaAsset::available() {
        return DynamicSourcedAsset::available() && m_Schema != nullptr;
    }

    JsonSchema* JsonSchemaAsset::getSchema() const {
        return m_Schema;
    }

    JsonAsset::JsonAsset(const std::string& document) {
        m_Document = loadJSONMemory(document);
    }

    JsonAsset::JsonAsset(const std::string& document, const JsonSchemaAsset* jsonSchema) {
        m_Document = loadJSONMemory(document, jsonSchema->getSchema());
    }

    JsonAsset::JsonAsset(const std::string& document, JsonSchema* jsonSchema) {
        m_Document = loadJSONMemory(document, jsonSchema);
    }


    JsonAsset::JsonAsset() {

    }

    rapidjson::Document& JsonAsset::getDocument() {
        return m_Document;
    }

    void JsonAsset::dispose() {
        
    }

    bool JsonAsset::available() {
        return m_Document.GetParseError() == rapidjson::ParseErrorCode::kParseErrorNone;
    }

    void JsonAsset::loads(const std::string& text) {
        m_Document.Parse(text.c_str());
    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const std::string& source, const AssetSource& src) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);
    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const char* source, const AssetSource& src) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);

    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const std::vector<char>& source, const AssetSource& src) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);

    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const std::string& source, const AssetSource& src, const JsonSchemaAsset* jsonSchema) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);

    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const char* source, const AssetSource& src, const JsonSchemaAsset* jsonSchema) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);
    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const std::vector<char>& source, const AssetSource& src, const JsonSchemaAsset* jsonSchema) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);
    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const std::string& source, const AssetSource& src, JsonSchema* jsonSchema) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);
    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const char* source, const AssetSource& src, JsonSchema* jsonSchema) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);
    }

    DynamicSourcedJsonAsset::DynamicSourcedJsonAsset(const std::vector<char>& source, const AssetSource& src, JsonSchema* jsonSchema) : DynamicSourcedAsset(source, src), JsonAsset() {
        if (DynamicSourcedAsset::available())
            loads(m_Content);
    }

    void DynamicSourcedJsonAsset::dispose() {
        JsonAsset::dispose();
        DynamicSourcedAsset::dispose();
    }

    bool DynamicSourcedJsonAsset::available() {
        return DynamicSourcedAsset::available() && JsonAsset::available();
    }

}