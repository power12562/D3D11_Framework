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
	std::vector<std::shared_ptr<GameObject>> objectList;

private:
	//Update
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void UpdateTransform();

	//Render
	void Render();

};