#include "CameraObject.h"
#include <Framework\SceneManager.h>

CameraObject::CameraObject()
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

