#pragma once
#include <Math\AssimpMath.h>
#include <Framework\Transform.h>

class SimpleMaterial;
class GameObject;
namespace Utility
{
	bool ParseFileName(aiString& str);
	std::vector<SimpleMaterial*> LoadFBX(const char* path, GameObject& _gameObject);
}