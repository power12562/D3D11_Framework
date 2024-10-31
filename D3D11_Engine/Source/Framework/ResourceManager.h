#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <Utility/utfConvert.h>

class Resource
{
public:
    static void ClearResourceManagers();
protected:
    inline static std::vector<std::function<void()>> clearList;
};

template <typename T>
class ResourceManager : public Resource
{
public:
    /*GetInstance*/
    static ResourceManager<T>& instance()
    {
        static ResourceManager<T> instance;
        return instance;
    }
private:
    ResourceManager();
    ~ResourceManager();
private:
	std::map<std::wstring, std::weak_ptr<T>> resourceMap;
public:
	void Clear();
	std::shared_ptr<T> GetResource(const wchar_t* name);
	std::shared_ptr<T> operator[](const wchar_t* key) { return GetResource(key); }
};

template<typename T>
ResourceManager<T>& GetResourceManager()
{
    return ResourceManager<T>::instance();
}

template<typename T>
inline ResourceManager<T>::ResourceManager()
{
    auto clearFunc = [this](){ this->Clear(); };
    clearList.push_back(clearFunc);
}

template<typename T>
inline ResourceManager<T>::~ResourceManager()
{
    Clear();
}

template<typename T>
inline void ResourceManager<T>::Clear()
{
    resourceMap.clear();
}

template <typename T>
std::shared_ptr<T> ResourceManager<T>::GetResource(const wchar_t* name)
{
    auto findIter = resourceMap.find(name);

    if (findIter != resourceMap.end())
    {
        if (std::shared_ptr<T> material = findIter->second.lock())
        {
            return  material;
        }
    }
    std::shared_ptr<T> material = std::make_shared<T>();
    resourceMap[name] = material;
    return material;
}
