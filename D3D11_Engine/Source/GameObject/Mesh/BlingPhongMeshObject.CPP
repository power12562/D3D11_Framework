#include "BlingPhongMeshObject.h"
#include <D3DCore/D3DConstBuffer.h>

void BlingPhongMeshObject::Start()
{
	Material = D3DConstBuffer::GetData<cb_BlingPhongMaterial>(GetNameToString().c_str());
}
