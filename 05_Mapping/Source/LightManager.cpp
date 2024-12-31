#include "LightManager.h"
#include <GameObject\Base\CameraObject.h>
#include <D3DCore/D3DRenderer.h>

LightManager::LightManager()
{
    if (!cbuffer)
    {
        cbuffer = std::make_unique<D3DConstBuffer>();
    }
}

LightManager::~LightManager()
{
    if (cbuffer)
    {
        cbuffer.reset();
    }
}

void LightManager::Start()
{
    cb_light = D3DConstBuffer::GetData<cb_DirectionalLight>(lightKey);
    int index = cbuffer->CreatePSConstantBuffers<cb_DirectionalLight>(lightKey);

    cb_bool = D3DConstBuffer::GetData<cbuffer_bool>(lightKey);
    index = cbuffer->CreatePSConstantBuffers<cbuffer_bool>(lightKey);
}

void LightManager::FixedUpdate()
{

}

void LightManager::Update()
{
    cb_light->CamPos = { Camera::GetMainCamera()->transform.position };
}

void LightManager::LateUpdate()
{

}
