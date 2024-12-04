#include "MeshRender.h"
#include <Framework/ResourceManager.h>
#include <Framework/HLSLManager.h>
#include <Utility/MemoryUtility.h>

using namespace Utility;

MeshRender::MeshRender()
{
}

MeshRender::~MeshRender()
{
	if (meshResource.use_count() == 1)
	{
		SafeRelease(meshResource->pIndexBuffer);
		SafeRelease(meshResource->pVertexBuffer);
	}
}

void MeshRender::SetMeshResource(const wchar_t* path)
{
	if (MeshID < 0)
	{
		__debugbreak();
		return;
	}

	using namespace utfConvert;
	if (meshResource == nullptr)
	{
		//고유의 fbx 경로 + index
		meshResource = GetResourceManager<DRAW_INDEX_DATA>().GetResource(path, MeshID);
	}
}

void MeshRender::CopyShader(MeshRender& rhs)
{
	pInputLayout = rhs.pInputLayout;
	pVertexShader = rhs.pVertexShader;
	pPixelShader = rhs.pPixelShader;
}

void MeshRender::SetVertexShader(const wchar_t* path)
{
	ResetVertexShader();
	hlslManager.CreateSharingShader(path, "vs_4_0", &pVertexShader, &pInputLayout);
}

void MeshRender::SetPixelShader(const wchar_t* path)
{
	ResetPixelShader();
	hlslManager.CreateSharingShader(path, "ps_4_0", &pPixelShader);
}

void MeshRender::ResetVertexShader()
{
	if (pVertexShader)
	{
		pInputLayout = nullptr;
		pVertexShader = nullptr;
	}
}

void MeshRender::ResetPixelShader()
{
	if (pPixelShader)
	{
		pPixelShader = nullptr;
	}
}

RENDERER_DRAW_DESC MeshRender::GetRendererDesc()
{
	RENDERER_DRAW_DESC desc{};
	desc.pConstBuffer = &constBuffer;
	desc.pD3DTexture2D = &texture2D;
	desc.pSamperState = &samplerState;
	desc.pInputLayout = pInputLayout;
	desc.pVertexShader = pVertexShader;
	desc.pPixelShader = pPixelShader;
	desc.pTransform = &gameObject.transform;
	desc.pVertexIndex = meshResource.get();
	return desc;
}

