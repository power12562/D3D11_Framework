#pragma once
#include <memory>
#include <Math\AssimpMath.h>
#include <Framework\Transform.h>
#include <functional>


class SimpleMaterial;
class Scene;
class GameObject;
namespace Utility
{
	bool ParseFileName(aiString& str);
	void LoadFBX(const wchar_t* path, 
		GameObject& _gameObject, 
		std::shared_ptr<SimpleMaterial> material,
		std::function<void(SimpleMaterial*)> initMaterial,
		bool isStatic);

	void LoadFBX(const wchar_t* path,
		GameObject& _gameObject,
		std::shared_ptr<SimpleMaterial> material,
		bool isStatic);

	void LoadFBXResource(const wchar_t* path);
}
