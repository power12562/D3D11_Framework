#include "LightManager.h"
#include <GameObject\Base\CameraObject.h>
#include <Framework\D3DRenderer.h>

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

void LightManager::Start()
{
    d3dRenderer.CreateConstantBuffers<cbuffer_Light>();
}

void LightManager::FixedUpdate()
{
}

void LightManager::Update()
{
    cb_Light.CamPos = { Camera::GetMainCamera()->transform.position };
    d3dRenderer.UpdateConstBuffer(cb_Light);
}

void LightManager::LateUpdate()
{
}