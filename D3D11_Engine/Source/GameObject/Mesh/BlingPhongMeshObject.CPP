#include "BlingPhongMeshObject.h"
#include <D3DCore/D3DConstBuffer.h>

void BlingPhongMeshObject::Start()
{
	SetMaterial(GetNameToString().c_str());
}

void BlingPhongMeshObject::SetMaterial(const char* name)
{
	materialName = name;
	material = D3DConstBuffer::GetData<cb_BlingPhongMaterial>(name);
}
