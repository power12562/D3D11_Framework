#include "MaterialManager.h"
#include <Material\SimpleMaterial.h>

MaterialManager& materialManager = MaterialManager::GetInstance();

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

void MaterialManager::Clear()
{
    materialMap.clear();
}

SimpleMaterial* MaterialManager::GetMaterial(const wchar_t* name)
{
    auto findIter = materialMap.find(name);

    if (findIter != materialMap.end())
    {
        return materialMap[name].get();
    }
    else
    {
        SimpleMaterial* material = new SimpleMaterial;
        materialMap[name].reset(material);
        return material;
    }
}
