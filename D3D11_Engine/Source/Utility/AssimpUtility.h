#pragma once
#include <memory>
#include <Math\AssimpMath.h>
#include <Core/Transform.h>
#include <functional>

class MeshRender;
class GameObject;

enum class SURFACE_TYPE
{
	BlingPhong,
	PBR,
	NONE
};

namespace Utility
{
	bool ParseFileName(aiString& str);

	void LoadFBX(const wchar_t* path, 
		GameObject& _gameObject, 
		std::function<void(MeshRender*)> initMesh,
		bool isStatic, 
		SURFACE_TYPE surface);

	void LoadFBX(const wchar_t* path,
		GameObject& _gameObject,
		bool isStatic,
		SURFACE_TYPE surface);

	void LoadFBXResource(const wchar_t* path, SURFACE_TYPE surface);
}
