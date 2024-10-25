#pragma once
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
		SimpleMaterial* material, 
		std::function<void(SimpleMaterial*)> initMaterial,
		bool);

	void LoadFBX(const char* path,
		GameObject& _gameObject,
		SimpleMaterial* material,
		bool isStatic);
}
