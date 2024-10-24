#pragma once
#include <Math\AssimpMath.h>
#include <Framework\Transform.h>
#include <functional>

class SimpleMaterial;
class GameObject;
namespace Utility
{
	bool ParseFileName(aiString& str);
	void LoadFBX(const char* path, GameObject& _gameObject, SimpleMaterial* material = nullptr, bool isStatic = false, 
		std::function<void(SimpleMaterial*)> initMaterial = [](SimpleMaterial* material)->void{ return; });
}
