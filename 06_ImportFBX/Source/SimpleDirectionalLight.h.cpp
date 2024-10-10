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
    d3dRenderer.CreateVSPSConstantBuffers<cbuffer_Light>();
    d3dRenderer.CreateVSPSConstantBuffers<cbuffer_bool>();
}

void SimpleDirectionalLight::FixedUpdate()
{
}

void SimpleDirectionalLight::Update()
{
    cb_Light.CamPos = { Camera::GetMainCamera()->transform.position };
    d3dRenderer.UpdateVSPSConstBuffer(cb_Light);
    d3dRenderer.UpdateVSPSConstBuffer(cb_bool);
}

void SimpleDirectionalLight::LateUpdate()
{
}
