#pragma once

#include "kat/core.hpp"
#include "kat/net.hpp"
#include <string_view>
#include <sstream>

namespace kat {
    struct AssetKey {
        const std::string name;
        const std::string_view assetType;

        inline bool operator==(const AssetKey& other) const {
            return name == other.name && assetType == other.assetType;
        };
    };
}

namespace std {
    template<>
    struct hash<kat::AssetKey> {
        inline std::size_t operator()(const kat::AssetKey& v) const {
            std::size_t h1 = std::hash<std::string>{}(v.name);
            std::size_t h2 = std::hash<std::string_view>{}(v.assetType);
            return h1 ^ h2;
        };
    };
}

namespace kat {

    
    class Asset {
    public:

        virtual ~Asset();

        virtual void dispose() = 0; 
        virtual bool available() = 0;
    };

    template<typename T>
    concept Asset_c = requires {
        T::type;
        requires std::is_same_v<const std::string_view, decltype(T::type)>;
    } && std::derived_from<T,Asset>;


    // dont use this ever.
    class AssetSubLibrary_base {
    public:


        inline Asset* getAsset_base(const std::string& id) {
            return m_Assets[id];
        };

    protected:
        std::unordered_map<std::string, Asset*> m_Assets;
    };

    template<Asset_c A>
    class AssetSubLibrary : public AssetSubLibrary_base {
    public:
        inline AssetSubLibrary() {

        };

        inline A* get(const std::string& id) {
            return reinterpret_cast<A*>(m_Assets[id]);
        };
        
        inline void set(const std::string& id, A* value) {
            m_Assets[id] = value;
        };

        inline bool has(const std::string& id) const {
            return m_Assets.contains(id);
        };

        inline A*& operator[](const std::string& id) {
            return reinterpret_cast<A*&>(m_Assets[id]);
        };
    };



    /**
     * Asset Collections store sets of asset keys and subcollections which can be used to group assets apart from just by type
     * 
     * Example Usage:
     *  An asset collection which contains all entity related assets, with a subcollection containing player assets, which contains player animations, sprites, sound effects.
     */
    class AssetCollection {
    public:
        bool contains(const kat::AssetKey& key);

        AssetCollection*& operator[](const std::string& id);
        void push(const std::string& id, AssetCollection* collection);

        void push(const kat::AssetKey& key);
        std::unordered_set<kat::AssetKey> getAssets();

        std::unordered_set<kat::AssetKey> getAssetsOfType(const std::string_view& typeID);
        
        template<Asset_c A>
        inline std::unordered_set<kat::AssetKey> getAssetsOfType() {
            return getAssetsOfType(A::type);
        };

        std::unordered_set<kat::AssetKey> getAssetsWithID(const std::string& id);


    private:
        std::unordered_set<kat::AssetKey> m_Assets;
        std::unordered_map<std::string_view, AssetCollection*> m_SubCollections;                
    };

    class AssetLibrary {
    public:
        AssetLibrary();

        AssetSubLibrary_base* getLibrary_base(const std::string_view& id);

        template<Asset_c A>
        inline AssetSubLibrary<A>* getLibrary() {
            return reinterpret_cast<AssetSubLibrary<A>*>(getLibrary_base(A::type));
        }

        AssetCollection* getCollection(const std::string& id);

    private:

        std::unordered_map<std::string_view, AssetSubLibrary_base*> m_Libraries;
        std::unordered_map<std::string, AssetCollection*> m_Collections;

    };




    class FileAsset : public Asset {
    public:

        static constexpr std::string_view type = "file";

        FileAsset(const std::string& filePath, bool binary = false);
        virtual ~FileAsset();

        std::vector<char> readCharArray();
        std::string readString();

        virtual void dispose() override;
        virtual bool available() override;
    
    protected:
        std::ifstream m_File;

        bool open = false;
    };

    class NetworkAsset : public Asset {
    public:

        static constexpr std::string_view type = "network";


        NetworkAsset(const std::string& url, bool downloadOnce = true);
        virtual ~NetworkAsset();

        std::vector<char> getTextCharArray();
        std::string getTextString();
        cpr::Response get();

        virtual void dispose() override;
        virtual bool available() override;

    protected:
        cpr::Url m_URL;
        bool m_CacheResponse;
        cpr::AsyncResponse m_CachedResponse;
    };

    enum class AssetSource {
        eURL, eFile, eMemory
    };

    class DynamicSourcedAsset : public Asset {
    public:
        static constexpr std::string_view type = "dynamic";

        DynamicSourcedAsset(const std::string& source, const AssetSource& src);
        DynamicSourcedAsset(const char* source, const AssetSource& src);
        DynamicSourcedAsset(const std::vector<char>& source, const AssetSource& src);

        virtual ~DynamicSourcedAsset();

        std::vector<char> getTextCharArray();
        std::string getTextString();

        virtual void dispose() override;
        virtual bool available() override;

    protected:

        bool m_Available = true;
        std::string m_Content;

    };

    class JsonSchemaAsset : public DynamicSourcedAsset {
    public:
        static constexpr std::string_view type = "json-schema";
        JsonSchemaAsset(const std::string& source, const AssetSource& src);
        JsonSchemaAsset(const char* source, const AssetSource& src);
        JsonSchemaAsset(const std::vector<char>& source, const AssetSource& src);

        virtual ~JsonSchemaAsset();

        JsonSchema* getSchema();

        virtual void dispose() override;
        virtual bool available() override;

    private:
        void disposeSchema();

        JsonSchema* m_Schema = nullptr;
    };

    class JsonAsset : public Asset {
    public:

        JsonAsset(const std::string& document);
        JsonAsset(const std::string& document, const JsonSchemaAsset* jsonSchema);
        JsonAsset(const std::string& document, const JsonSchema* jsonSchema);
        JsonAsset();

        static constexpr std::string_view type = "json-memory";

        rapidjson::Document& getDocument();
        
        virtual void dispose() override;
        virtual bool available() override;

    protected:
        rapidjson::Document m_Document;
    };

    class DynamicSourcedJsonAsset : public DynamicSourcedAsset, public JsonAsset {
    public:
        static constexpr std::string_view type = "json-dynamic";



        virtual void dispose() override;
        virtual bool available() override;

    };


    class AssetManager {

    public:
        AssetManager();


        Asset* getAsset(const std::string& typeID, const std::string& id);

        template<Asset_c A>
        inline A* getAsset(const std::string& id) {
            return m_Library->getLibrary<A>()->get(id);
        };

        FileAsset* getFileAsset(const std::string &id);

    private:

        AssetLibrary* m_Library;

    };

    template<Asset_c A>
    inline constexpr std::string_view getTypeID(const A& obj) {
        return A::type;
    };

    template<Asset_c A>
    inline constexpr std::string_view getTypeID(const A*& obj) {
        return A::type;
    };
}