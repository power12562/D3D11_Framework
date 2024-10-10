#include "D3DConstBuffer.h"

D3DConstBuffer::D3DConstBuffer()
{
    CreateVSPSConstantBuffers<cbuffer_Transform>();
    CreateVSPSConstantBuffers<cbuffer_Camera>();
}

D3DConstBuffer::~D3DConstBuffer()
{
    using namespace Utility;
    for (auto& cbuffer : vs_cbufferList)
    {
        SafeRelease(cbuffer);
    }
    for (auto& cbuffer : ps_cbufferList)
    {
        SafeRelease(cbuffer);
    }
}

D3DConstBuffer::REG_INDEX D3DConstBuffer::CreateVSPSConstantBuffers(const char* key, unsigned int buffer_size)
{
    REG_INDEX index{};
    index.vs_index = CreateVSConstantBuffers(key, buffer_size);
    index.ps_index = CreatePSConstantBuffers(key, buffer_size);

    return index;
}

int D3DConstBuffer::CreateVSConstantBuffers(const char* key, unsigned int buffer_size)
{
    assert((buffer_size % 16) == 0 && "Constant Buffer size must be 16 - byte aligned");

    if (vs_cbufferList.size() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
    {
        __debugbreak(); //��� ���� �ִ� ���� ����.
        return -1;
    }

    if (vs_cbufferMap.find(key) != vs_cbufferMap.end())
    {
        __debugbreak(); //�̹� �����ϴ� Ű��.
        return -1;
    }

    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ��� ���۴� dynamic���� �����ϰ� ���� ����.
    bufferDesc.ByteWidth = buffer_size;  // ��� ���� ũ��
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    int regIndex = (int)vs_cbufferList.size();
    vs_cbufferMap[key] = regIndex;

    ID3D11Buffer* cBufferTemp{};
    Utility::CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
    vs_cbufferList.push_back(cBufferTemp);

    return regIndex;
}

int D3DConstBuffer::CreatePSConstantBuffers(const char* key, unsigned int buffer_size)
{
    assert((buffer_size % 16) == 0 && "Constant Buffer size must be 16 - byte aligned");

    if (ps_cbufferList.size() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
    {
        __debugbreak(); //��� ���� �ִ� ���� ����.
        return -1;
    }

    if (ps_cbufferMap.find(key) != ps_cbufferMap.end())
    {
        __debugbreak(); //�̹� �����ϴ� Ű��.
        return -1;
    }

    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ��� ���۴� dynamic���� �����ϰ� ���� ����.
    bufferDesc.ByteWidth = buffer_size;  // ��� ���� ũ��
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    int regIndex = (int)ps_cbufferList.size();
    ps_cbufferMap[key] = regIndex;

    ID3D11Buffer* cBufferTemp{};
    Utility::CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
    ps_cbufferList.push_back(cBufferTemp);

    return regIndex;
}