#include "Camera.h"
#include <Framework/D3DRenderer.h>
#include <Framework/D3D11_GameApp.h>
#include <Math/Mathf.h>

using namespace DirectX;

Camera::Camera()
{

}

Camera::~Camera()
{
	if (this == mainCam)
	{
		mainCam = nullptr;
	}
}

const Matrix& Camera::GetVM() const
{
	return view;
}

const Matrix& Camera::GetPM() const
{
	return projection;
}

void Camera::Start()
{

}

void Camera::FixedUpdate()
{

}

void Camera::Update()
{
	view = XMMatrixLookToLH(transform.position, transform.Front, transform.Up);
	 
	const SIZE& size = D3D11_GameApp::GetClientSize();
	projection = XMMatrixPerspectiveFovLH(FOV * Mathf::Deg2Rad, (FLOAT)size.cx / (FLOAT)size.cy, Near, Far);
}

void Camera::LateUpdate()
{

}



