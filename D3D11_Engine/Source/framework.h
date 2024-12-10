#pragma once
//frameworks
#include <Framework/D3D11_GameApp.h>
#include <Framework/DXTKInputSystem.h>
#include <Framework/HLSLManager.h>
#include <Framework/ImguiHelper.h>
#include <Framework/ResourceManager.h>
#include <Framework/SceneManager.h>
#include <Framework/TextureManager.h>
#include <Framework/TimeSystem.h>

//gameObjects
#include <GameObject/Base/CameraObject.h>
#include <GameObject/Mesh/BlingPhongMeshObject.h>
#include <GameObject/Mesh/PBRMeshObject.h>
#include <GameObject/Mesh/SphereObject.h>
#include <GameObject/Mesh/SkyBoxObject.h>

//Components
#include <Component/BoneComponent.h>
#include <Component/Camera/Camera.h>
#include <Component/Camera/CameraMoveHelper.h>
#include <Component/TransformAnimation.h>

#include <Component/Render/BlingPhongBoneMeshRender.h>
#include <Component/Render/BlingPhongMeshRender.h>
#include <Component/Render/PBRBoneMeshRender.h>
#include <Component/Render/PBRMeshRender.h>
#include <Component/Render/SkyBoxRender.h>

//Lights
#include <Light/PBRDirectionalLight.h>
#include <Light/SimpleDirectionalLight.h>

//Utility
#include <Utility/AssimpUtility.h>
#include <Utility/Console.h>
#include <Utility/D3D11Utility.h>
#include <Utility/SpinLock.h>
#include <Utility/utfConvert.h>
