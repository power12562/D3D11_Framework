#pragma once
#include <memory>
#include <Math\AssimpMath.h>
#include <Framework\Transform.h>
#include <functional>


class SimpleMaterial;
class GameObject;
namespace Utility
{
	bool ParseFileName(aiString& str);
	void LoadFBX(const char* path, 
		GameObject& _gameObject, 
		std::shared_ptr<SimpleMaterial> material,
		std::function<void(SimpleMaterial*)> initMaterial,
		bool);

	void LoadFBX(const char* path,
		GameObject& _gameObject,
		std::shared_ptr<SimpleMaterial> material,
		bool isStatic);

	void CopyFBX(GameObject* DestinationObj, GameObject* SourceObj, const wchar_t* path,
		std::shared_ptr<SimpleMaterial> material,
		std::function<void(SimpleMaterial*)> initMaterial);
}
