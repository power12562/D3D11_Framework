#pragma once
#include <GameObject/Base/GameObject.h>

class SkyBoxRender;
class SkyBoxObject : public GameObject
{
public:
	SkyBoxObject();
	virtual ~SkyBoxObject() override = default;

	SkyBoxRender& skyBoxRender;
};