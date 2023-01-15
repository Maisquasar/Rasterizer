#pragma once
#include <vector>
#include <Renderer.hpp>
#include <Light.hpp>
#include <Model.hpp>
#include <Camera.hpp>
#include <prng.hpp>
#define Max_version 10

class Scene
{
private:
    std::vector<Light> lights;
    
public:
    double time = 0.0;
    int64_t timeS = 0;
    size_t index = 0;
    int version = 1;
    std::vector<Model> models;
    std::vector<Texture> mTexture;
    bool showLights = false;
    PRNG rng = PRNG();
    
    Scene();
    Scene(Renderer&);
    ~Scene();

    void Update(float p_deltaTime, Renderer& p_renderer, const Vec3D& cameraFocus, CameraInputs& input);
    // Update when slider is moved.
    void UpdateVersion(const int& newVersion, Renderer& p_renderer, CameraInputs& input, int& msaaLevel);
    // Load all textures in the file "assets/textures/".
    void LoadAllTexture();

    int getNumberOfTriangles(bool scene = false);
    // Return the texture id by the name given.
    int getTextureIdByName(const char* name);
private:
    // Show the lights Ui.
    void ShowImGuiControls();

};


