#pragma once

#include <string>
#include <maths.hpp>
#include <Vertices.hpp>
#include <vector>
#include <Texture.hpp>
#include <Renderer.hpp>
#include <Mesh.hpp>
#include <modelLoader.hpp>

class Model
{
private:
    std::string ModelName = "";
public:
    std::vector<Mesh*> meshs;
    bool bilinear = false;
    Vec3D pos = {0};
    Vec3D rot = {0};
    Vec3D scale = {1};
    Mat4D model = Mat4D::Identity();
    UChar4D color = UChar4D(0xffffffff);
    int modelShader = 0;    //the Shader index of the Model.

    Model();
    ~Model();

    void draw(Renderer &renderer, const std::vector<Texture> &textures, const std::vector<Light>& lights);
    // Delete the model.
    void deleteModel();
    // Load a Mesh from a .obj file.
    void LoadMesh(const char* path, int texture, bool out = true);

    //Set The matrix of the model by the matrix given.
    void SetModel(Mat4D *p_modelMatrix);
    void SetModelName(std::string);
    //Set The matrix of the model by the given parameters.
    void SetMatrix(Vec3D pos, Vec3D rot = {0}, Vec3D scale = {1});

    std::string getModelName() {return ModelName;}
    int getNumberOfTriangles(bool scene = false);

};
