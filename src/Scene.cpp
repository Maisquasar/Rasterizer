#include <imgui.h>
#include <maths.hpp>
#include <Model.hpp>
#include <Scene.hpp>
#include <bmpLoader.hpp>
#include <modelLoader.hpp>

#ifdef WINDOWS
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <dirent.h>
#endif // WINDOWS

#include <iostream>
#include <vector>
Scene::Scene() {}
Scene::Scene(Renderer &renderer)
{
    // Init Light 0.
    lights.push_back(Light(Vec3D(), Vec3D(1), Vec3D(), Vec3D()));
    models.resize(11);

    // Load Texture.
    LoadAllTexture();

    // Load Models/
    models[0].LoadMesh("assets/Models/castle.obj", getTextureIdByName("castle"), false);
    models[1].LoadMesh("assets/Models/daway.obj", getTextureIdByName("daway"), false);
    models[1].LoadMesh("assets/Models/daway2.obj", getTextureIdByName("daway2"), false);
    models[2].LoadMesh("assets/Models/Mario.obj", getTextureIdByName("Mario"), false);
    models[3].LoadMesh("assets/Models/craftsman.obj", getTextureIdByName("craftsman"));
    models[3].LoadMesh("assets/Models/anvil.obj", getTextureIdByName("anvil"));
    models[3].LoadMesh("assets/Models/floor.obj", getTextureIdByName("floor"));
    models[3].LoadMesh("assets/Models/particles.obj", getTextureIdByName("particles"));
    models[7].LoadMesh("assets/Models/fish_sphere.obj", getTextureIdByName("fish"));
    models[10].LoadMesh("assets/Models/light.obj", -1);

    // Set parameters of models.
    models[10].scale = Vec3D(0.2f);

    //Create Basic Models.
    models[4].meshs.push_back(new Mesh{CreateNewTriangle(Vec3D{-0.5f, -0.5f, 0.0f}, Vec3D{0.5f, -0.5f, 0.0f}, Vec3D{0.0f, 0.5f, 0.0f})});
    models[4].meshs[0]->setVisibility(false);

    models[5].meshs.push_back(new Mesh{CreateNewSphere(1, 10, 10, Vec4D(0, 0, 1, 1))});
    models[5].SetMatrix(Vec3D(0.5f, 0.0f, 2.0f), Vec3D(0, 0, 0), Vec3D(1));
    models[5].meshs[0]->setVisibility(false);

    models[6].LoadMesh("assets/Models/plane.obj", getTextureIdByName("water"));
    models[6].modelShader = 1; // Sea waves shader

    models[8].meshs.push_back(new Mesh{CreateNewCube(Vec4D(1, 0, 0, 1))});
    models[8].SetMatrix(Vec3D(-0.5f, 0.0f, 2.0f), Vec3D(0, 0, 0), Vec3D(1));
    models[8].meshs[0]->setVisibility(false);

    models[9].meshs.push_back(new Mesh{CreateNewCube(Vec4D(1, 1, 0, 0.5))});
    models[9].SetMatrix(Vec3D(-0.5f, 0.0f, 4.0f), Vec3D(0, 0, 0), Vec3D(1));
    models[9].meshs[0]->setVisibility(false);

    // Check if Models have same name:
    std::vector<size_t> same;
    for (size_t i = 0; i < models.size(); i++)
    {
        for (size_t j = 0; j < models.size(); j++)
        {
            if (models[i].meshs[0]->getName() == models[j].meshs[0]->getName() && j != i)
            {
                bool samei = false, samej = false;
                for (size_t x = 0; x < same.size(); x++)
                {
                    if (same[x] == i)
                        samei = true;
                    if (same[x] == j)
                        samej = true;
                }
                if (!samei)
                    same.push_back(i);
                if (!samej)
                    same.push_back(j);
            }
        }
    }
    std::string tmp;
    // If same name, the mesh[0] change name
    for (size_t i = 0; i < same.size(); i++)
    {
        tmp.clear();
        tmp = models[same[i]].meshs[0]->getName();
        tmp.push_back('0' + i);
        models[same[i]].meshs[0]->SetMeshName(tmp);
    }
}

Scene::~Scene()
{
    for (size_t i = 0; i < mTexture.size(); i++)
    {
        mTexture[i].deleteTexture();
    }
    for (size_t i = 0; i < models.size(); i++)
    {
        models[i].deleteModel();
    }
}

void Scene::Update(float deltaTime, Renderer &renderer, const Vec3D &cameraFocus, CameraInputs &input)
{
    if (ImGui::Begin("Scene"))
    {
        renderer.ShowImGuiControls();
        ShowImGuiControls();
    }
    ImGui::End();
    Mat4D matrix;
    // Version updater.
    if (version >= 4)
    {
        models[5].SetMatrix(Vec3D(0.5f, 0.0f, 2.0f), Vec3D(0, 30 * time, 0), Vec3D(1));
        models[8].SetMatrix(Vec3D(-0.5f, 0, 2.0f), Vec3D(0, 30 * time, 0), Vec3D(1));
    }
    if (version >= 8)
    {
        models[9].SetMatrix(Vec3D(-0.5f, 0.0f, 4.0f), Vec3D(0, 30 * time, 0), Vec3D(0.5f));
    }
    if (version >= 9)
    {
        lights[0].mPos = cameraFocus;
        models[8].SetMatrix(Vec3D(-0.5f, 0.1*sinf(time), 2.0f), Vec3D(5*sinf(time*0.97f+3.7f), 0, 5*sinf(time*1.018f+1.1f)), Vec3D(1));
    }

    time += deltaTime;
    int64_t sec = (int64_t)(time*10);
    // Draw all Models.
    if (sec != timeS)
    {
        renderer.rngState = rng.getRandomNumber();
    }
    for (size_t i = 0; i < models.size(); i++)
    {
        models[i].draw(renderer, mTexture, lights);
    }
    if (showLights)
    {
        // Draw Sphere on light pos.
        models[10].meshs[0]->setVisibility(true);
        for (size_t i = 0; i < lights.size(); i++)
        {
            models[10].pos = lights[i].mPos;
            models[10].draw(renderer, mTexture, lights);
        }
        models[10].meshs[0]->setVisibility(false);
    }
    timeS = sec;
}

void Scene::ShowImGuiControls()
{
    if (ImGui::CollapsingHeader("Lights", ImGuiTreeNodeFlags_Selected))
    {
        if (index >= lights.size()) index = lights.size()-1;
        char Name[32];
        bool selected = false;
        sprintf(Name, "Light %zu", index);
        if (ImGui::BeginCombo("Light", Name))
        {
            for (size_t i = 0; i < lights.size(); i++)
            {
                sprintf(Name, "Light %zu", i);
                if (ImGui::Selectable(Name, selected))
                {
                    index = i;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::ColorEdit3("Ambient color", &lights[index].ambientComponent.x);
        ImGui::ColorEdit3("Diffuse color", &lights[index].diffuseComponent.x);
        ImGui::ColorEdit3("Specular color", &lights[index].specularComponent.x);
        if (ImGui::Button("Add light"))
        {
            lights.push_back(Light(lights[0]));
            index = lights.size()-1;
        }
        if (ImGui::Button("Delete light"))
        {
            if (lights.size() <= 1)
                lights[0] = Light();
            else
            {
                // Delete the current light.
                lights.erase(lights.begin() + index);
            }
        }
        ImGui::Checkbox("Show Lights",&showLights);
    }
}

int Scene::getNumberOfTriangles(bool scene)
{
    int x = 0;
    for (int i = 0; i < (int)models.size(); i++)
    {
        // Increment numbers of triangle of every meshs of model.
        x += models[i].getNumberOfTriangles(scene);
    }
    
    return x;
}

void Scene::UpdateVersion(const int &newVersion, Renderer &renderer, CameraInputs &input, int &msaaLevel)
{
    Mat4D matrix;
    if (newVersion != 10)
    {
        for (size_t i = 0; i < models.size(); i++)
        {
            for (size_t j = 0; j < models[i].meshs.size(); j++)
            {
                models[i].meshs[j]->setVisibility(false);
            }
        }
        lights.clear();
        lights.push_back(Light());
        if (newVersion >= 3)
            lights[0] = Light(Vec3D(), Vec3D(0.2f), Vec3D(0.4f), Vec3D(0.4f));
    }
    input.ortho = (newVersion < 5);
    renderer.WireFrame = (newVersion == 7);
    if (newVersion > 7)
        for (size_t i = 0; i < models.size(); i++){
            models[i].bilinear = true;
        }
    renderer.perPixelLight = (newVersion > 3);
    msaaLevel = (newVersion == 10) ? 2 : 1;
    switch (newVersion)
    {
    case 10:
        models[2].modelShader = 2; // Cel-shading
        models[2].pos = Vec3D(2,1,0);
        models[2].meshs[0]->setVisibility(true);
        models[6].meshs[0]->setVisibility(true);
        models[8].meshs[0]->SetTexture(getTextureIdByName("crate"));
        models[8].meshs[0]->setVisibility(true);
        lights.clear();
        lights.push_back(Light(false));
        lights.push_back(Light(Vec3D(-4.912,14.844,-4.472),Vec3D(154/255.0f),Vec3D(190/255.0f),Vec3D(146/255.0f)));
        break;
    case 9:
        models[9].meshs[0]->setVisibility(false);
        models[8].meshs[0]->setVisibility(false);
        models[2].modelShader = 0;
        models[2].meshs[0]->setVisibility(true);
        break;
    case 8:
        models[8].meshs[0]->SetTexture(getTextureIdByName("crate"));
        models[8].meshs[0]->setVisibility(true);
        models[9].meshs[0]->setVisibility(true);
        break;
    case 7:
    case 6:
        models[8].meshs[0]->SetTexture(getTextureIdByName("crate"));
        models[8].meshs[0]->setVisibility(true);
        break;
    case 5:
    case 4:
    case 3:
    case 2:
        models[5].meshs[0]->setVisibility(true);
        models[8].meshs[0]->SetTexture(-1);
        models[8].meshs[0]->setVisibility(true);
        break;
    case 1:
        models[4].meshs[0]->setVisibility(true);
        break;
    default:
        break;
    }
    version = newVersion;
}

void Scene::LoadAllTexture()
{
    std::vector<std::string> files;
#ifdef WINDOWS
    std::string path = "assets/Textures/";
    for (const auto& entry : fs::directory_iterator(path))
        files.push_back(entry.path().generic_string());
#else
    DIR* dir;
    struct dirent* diread;

    if ((dir = opendir("assets/Textures/")) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            if (diread->d_name[0] != '.')
                files.push_back(diread->d_name);
        }
        closedir(dir);
    }
    else
    {
        perror("opendir");
    }
#endif // WINDOWS

    for (size_t i = 0; i < files.size(); ++i)
    {
        char tmp[64];
#ifdef WINDOWS
        sprintf(tmp, "%s", files[i].data());
#else
        sprintf(tmp, "assets/Textures/%s", files[i].data());
#endif // WINDOWS
        mTexture.push_back(Texture(tmp));
    }
}

int Scene::getTextureIdByName(const char *name)
{
    for (size_t i = 0; i < mTexture.size(); i++)
    {
        if (!strcmp(mTexture[i].getName().c_str(), name))
        {
            return mTexture[i].getTextureID() - 2;
        }
    }
    return -1;
}
