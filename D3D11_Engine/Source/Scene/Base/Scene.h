#pragma once
#include <string>
#include <vector>
#include <memory>
#include <GameObject\Base\GameObject.h>

class Scene
{
	friend class SceneManager;
public:
	Scene();
	virtual ~Scene();

protected:
	std::vector<std::unique_ptr<GameObject>> objectList;

private:
	void FixedUpdate();
	void Update();
	void LateUpdate();
};