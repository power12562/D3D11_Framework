#include "Camera.h"
#include <D3DCore/D3DRenderer.h>
#include <D3DCore/D3D11_GameApp.h>
#include <Math/Mathf.h>

using namespace DirectX;

Camera::Camera()
{
	FOV = 90;
	Near = 0.5f;
	Far = 10000.f;
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

const Matrix& Camera::GetIVM() const
{
	return inversView;
}

const Matrix& Camera::GetPM() const
{
	return projection;
}

const Matrix& Camera::GetIPM() const
{
	return inversProjection;
}

Vector3 Camera::ScreenToWorldPoint(float screenX, float screenY, float distance)
{
	SIZE clientSize = D3D11_GameApp::GetClientSize();

	float mouseX = screenX;
	float mouseY = screenY;
	float screenWidth = (float)clientSize.cx;
	float screenHeight = (float)clientSize.cy;

	//���콺 ��ǥ NDC ��ȯ
	float ndcX = (2.0f * mouseX) / screenWidth - 1.0f;
	float ndcY = 1.0f - (2.0f * mouseY) / screenHeight; // y�� ���� ó��
	float ndcZ = (distance - Near) / (Far - Near);
	XMFLOAT3 mouseNDC(ndcX, ndcY, ndcZ);

	//���� ��ǥ�� ��ȯ
	XMVECTOR mouseWorld = XMVector3TransformCoord(XMLoadFloat3(&mouseNDC), inversProjection);
	mouseWorld = XMVector3TransformCoord(mouseWorld, inversView);

	return mouseWorld;
}

Ray Camera::ScreenPointToRay(int pointX, int pointY)
{
	SIZE clientSize = D3D11_GameApp::GetClientSize();

	// ȭ�� �������� ���콺 Ŭ�� ��ġ (������ ��ǥ)
	float mouseX = (float)pointX; // ���콺 X ��ǥ
	float mouseY = (float)pointY; // ���콺 Y ��ǥ
	float screenWidth = (float)clientSize.cx; // ȭ�� �ʺ�
	float screenHeight = (float)clientSize.cy; // ȭ�� ����

	// NDC�� ��ȯ (x, y�� -1���� 1 ���� ������ ��ȯ)
	float ndcX = (2.0f * mouseX) / screenWidth - 1.0f;
	float ndcY = 1.0f - (2.0f * mouseY) / screenHeight; // y�� ���� ó��
	float nearZ = 0.f;
	float farZ = 1.0f;
	XMFLOAT3 nearPoint(ndcX, ndcY, nearZ);
	XMFLOAT3 farPoint(ndcX, ndcY, farZ);

	//���� ��ǥ�� ��ȯ
	XMVECTOR nearWorld = XMVector3TransformCoord(XMLoadFloat3(&nearPoint), inversProjection);
	nearWorld = XMVector3TransformCoord(nearWorld, inversView);

	XMVECTOR farWorld = XMVector3TransformCoord(XMLoadFloat3(&farPoint), inversProjection);
	farWorld = XMVector3TransformCoord(farWorld, inversView);

	// ������ ���� ���� ���
	FXMVECTOR Origin = nearWorld;
	FXMVECTOR Direction = XMVector3Normalize(XMVectorSubtract(farWorld, nearWorld));

	Ray ray(Origin, Direction);
	return ray;
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
	inversView = XMMatrixInverse(nullptr, view);
	 
	const SIZE& size = D3D11_GameApp::GetClientSize();
	projection = XMMatrixPerspectiveFovLH(FOV * Mathf::Deg2Rad, (FLOAT)size.cx / (FLOAT)size.cy, Near, Far);
	inversProjection = XMMatrixInverse(nullptr, projection);
}

void Camera::LateUpdate()
{

}



