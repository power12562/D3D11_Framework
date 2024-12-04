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
    cbuffer.CreatePSConstantBuffers<cb_Light>();
    cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
}

void LightManager::FixedUpdate()
{
}

void LightManager::Update()
{
    cb_light.CamPos = { Camera::GetMainCamera()->transform.position };
    cbuffer.UpdateConstBuffer(cb_light);
    cbuffer.UpdateConstBuffer(cb_bool);
}

void LightManager::LateUpdate()
{
}
