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
    index = d3dRenderer.CreatePSConstantBuffers<cbuffer_Light>();
    index = d3dRenderer.CreatePSConstantBuffers<cbuffer_bool>();
    index = d3dRenderer.CreatePSConstantBuffers<cb_localBool>();
}

void SimpleDirectionalLight::FixedUpdate()
{
}

void SimpleDirectionalLight::Update()
{
    cb_Light.CamPos = { Camera::GetMainCamera()->transform.position };
    d3dRenderer.UpdatePSConstBuffer(cb_Light);
    d3dRenderer.UpdatePSConstBuffer(cb_bool);
}

void SimpleDirectionalLight::LateUpdate()
{
}
