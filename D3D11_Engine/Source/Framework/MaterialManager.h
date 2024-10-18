#pragma once
#include <Framework\TSingleton.h>
#include <map>
#include <string>
#include <memory>

class SimpleMaterial;
class MaterialManager;
extern MaterialManager& materialManager;

class MaterialManager : public TSingleton<MaterialManager>
{
	friend class  TSingleton;
private:
	MaterialManager();
	~MaterialManager();

	std::map<std::wstring, std::unique_ptr<SimpleMaterial>> materialMap;
public:
	void Clear();
	SimpleMaterial* GetMaterial(const wchar_t* name);

	SimpleMaterial* operator[](const wchar_t* key) { return GetMaterial(key); }
};
