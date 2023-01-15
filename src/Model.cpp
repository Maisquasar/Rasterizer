#include <Model.hpp>
#ifdef WINDOWS
#include <Sleep.hpp>
#endif // WINDOWS

Model::Model()
{
    // Set the default name of model.
    static int id = -1;
    id++;
    char name[32];
    sprintf(name, "Model %d", id);
    for (int i = 0; i < 32; i++)
    {
        if (name[i] == '\0')
            break;
        ModelName.push_back(name[i]);
    }
}
Model::~Model() {}

void Model::draw(Renderer &renderer, const std::vector<Texture> &textures, const std::vector<Light> &lights)
{
    // Set matrix of model.
    Mat4D matrix = Mat4D::CreateTransformMatrix(pos, rot, scale);
    renderer.SetModel(&matrix);

    //Set color.
    renderer.SetColor(color);
    renderer.shaderselected = modelShader;
    // Render all meshs.
    for (unsigned int i = 0; i < meshs.size(); i++)
    {
        meshs[i]->draw(renderer, textures, lights, bilinear);
    }
}

void Model::SetMatrix(Vec3D pos, Vec3D rot, Vec3D scale)
{
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
}

int Model::getNumberOfTriangles(bool scene)
{
    int x = 0;
        for (int i = 0; i < (int)meshs.size(); i++)
        {
            if (!scene)
                x += meshs[i]->getNumberOfTriangles();
            else if (meshs[i]->selected == true)
                x += meshs[i]->getNumberOfTriangles();
        }
    return x;
}

void Model::LoadMesh(const char *path, int texture, bool out)
{
    // Create new mesh
    meshs.push_back(new Mesh());
    meshs.back()->setVisibility(out);
    // Set mesh to loaded mesh.
    Loader::LoadMesh(path, meshs.back(), texture);
}

void Model::deleteModel()
{
    for (size_t i = 0; i < meshs.size(); i++)
    {
        while (!meshs[i]->loaded)
        {
#ifdef WINDOWS
            nanosleep((long long)(1000000000 / 20));
#else
            timespec time = { 0 };
            time.tv_nsec = (long)(1000000000 / 20);
            nanosleep(&time, NULL);
#endif // WINDOWS
        }
        delete meshs[i];
    }
}