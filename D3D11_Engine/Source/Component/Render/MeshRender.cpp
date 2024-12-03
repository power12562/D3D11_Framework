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
	SafeRelease(pInputLayout);
	SafeRelease(pVertexShader);
	SafeRelease(pPixelShader);
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

void MeshRender::SetVertexShader(const wchar_t* path)
{
	SafeRelease(pInputLayout);
	SafeRelease(pVertexShader);
	hlslManager.CreateSharingShader(path, "vs_4_0", &pVertexShader, &pInputLayout);
}

void MeshRender::SetPixelShader(const wchar_t* path)
{
	SafeRelease(pPixelShader);
	hlslManager.CreateSharingShader(path, "ps_4_0", &pPixelShader);
}

