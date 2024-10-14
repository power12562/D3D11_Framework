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
    cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
    cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
}

void LightManager::FixedUpdate()
{
}

void LightManager::Update()
{
    cb_Light.CamPos = { Camera::GetMainCamera()->transform.position };
    cbuffer.UpdateConstBuffer(cb_Light);
    cbuffer.UpdateConstBuffer(cb_bool);
}

void LightManager::LateUpdate()
{
}
