#include "SimpleDirectionalLight.h"
#include <GameObject\Base\CameraObject.h>
#include <Framework\D3DRenderer.h>

SimpleDirectionalLight::SimpleDirectionalLight()
{
}

SimpleDirectionalLight::~SimpleDirectionalLight()
{
}

void SimpleDirectionalLight::Start()
{
    int index;
    index = cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
    index = cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
    index = cbuffer.CreatePSConstantBuffers<cb_localBool>();

}

void SimpleDirectionalLight::FixedUpdate()
{
}

void SimpleDirectionalLight::Update()
{
    cb_Light.CamPos = { Camera::GetMainCamera()->transform.position };
    cbuffer.UpdateConstBuffer(cb_Light);
    cbuffer.UpdateConstBuffer(cb_bool);
}

void SimpleDirectionalLight::LateUpdate()
{
}
