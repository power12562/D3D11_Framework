#pragma once
#include <memory>
#include <Math\AssimpMath.h>
#include <Framework\Transform.h>
#include <functional>


class SimpleMaterial;
class MeshRender;
class Scene;
class GameObject;

namespace Utility
{
	bool ParseFileName(aiString& str);
	void LoadFBX(const wchar_t* path, 
		GameObject& _gameObject, 
		std::function<void(MeshRender*)> initMesh,
		bool isStatic);

	void LoadFBX(const wchar_t* path,
		GameObject& _gameObject,
		bool isStatic);

	void LoadFBXResource(const wchar_t* path);
}
