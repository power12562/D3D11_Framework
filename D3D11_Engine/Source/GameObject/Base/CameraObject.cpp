#include "CameraObject.h"

CameraObject::CameraObject(const wchar_t* name) : GameObject(name)
{
	Camera& cam = AddComponent<Camera>();
}

CameraObject::~CameraObject()
{


}

