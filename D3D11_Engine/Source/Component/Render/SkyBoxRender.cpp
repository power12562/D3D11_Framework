#include "SkyBoxRender.h"

void SkyBoxRender::Start()
{
    // Front face
    vertices.emplace_back(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f));
    vertices.emplace_back(Vector4(1.0f, -1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f));
    vertices.emplace_back(Vector4(1.0f, 1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f));
    vertices.emplace_back(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f));

    // Back face
    vertices.emplace_back(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f));
    vertices.emplace_back(Vector4(1.0f, -1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f));
    vertices.emplace_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f));
    vertices.emplace_back(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f));

    // Indices for the cube (two triangles per face)
    // Front face
    indices.emplace_back(0); indices.emplace_back(1); indices.emplace_back(2);
    indices.emplace_back(0); indices.emplace_back(2); indices.emplace_back(3);

    // Back face
    indices.emplace_back(4); indices.emplace_back(6); indices.emplace_back(5);
    indices.emplace_back(4); indices.emplace_back(7); indices.emplace_back(6);

    // Left face
    indices.emplace_back(4); indices.emplace_back(5); indices.emplace_back(1);
    indices.emplace_back(4); indices.emplace_back(1); indices.emplace_back(0);

    // Right face
    indices.emplace_back(3); indices.emplace_back(2); indices.emplace_back(6);
    indices.emplace_back(3); indices.emplace_back(6); indices.emplace_back(7);

    // Top face
    indices.emplace_back(1); indices.emplace_back(5); indices.emplace_back(6);
    indices.emplace_back(1); indices.emplace_back(6); indices.emplace_back(2);

    // Bottom face
    indices.emplace_back(4); indices.emplace_back(0); indices.emplace_back(3);
    indices.emplace_back(4); indices.emplace_back(3); indices.emplace_back(7);
    CreateMesh();

    texture2D.resize(1); //Å¥ºê¸Ê¿ë ÇÏ³ª¸¸ ¾¸
    ResetSkyBox();


}

void SkyBoxRender::SetSkyBox(const wchar_t* path)
{
    texture2D.SetTexture2D(0, path);
}

void SkyBoxRender::ResetSkyBox()
{
    texture2D.SetDefaultTexture(0, E_TEXTURE_DEFAULT::ONE);
}
