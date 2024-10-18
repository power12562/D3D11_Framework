#pragma once
#include <Math\AssimpMath.h>
#include <Framework\Transform.h>

class SimpleMaterial;
class GameObject;
namespace Utility
{
	bool ParseFileName(aiString& str);
	void LoadFBX(const char* path, GameObject& _gameObject, bool isStatic = false, SimpleMaterial* material = nullptr);
	void LoadFBX(const char* path, GameObject& _gameObject, SimpleMaterial* material);
}
