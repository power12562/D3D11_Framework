#include "CameraObject.h"

CameraObject::CameraObject(const wchar_t* name) : GameObject(name)
{
	cam = &AddComponent<Camera>();
}

CameraObject::~CameraObject()
{


}

void CameraObject::SetMainCamera()
{
	cam->SetMainCamera();
}

