#include <Framework/ResourceManager.h>

void Resource::ClearResourceManagers()
{
	for (auto& clear : clearList)
	{
		clear();
	}
}
