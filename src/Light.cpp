#include <Light.hpp>

Light::Light(Vec3D pos, LightComponent ambient, LightComponent diffuse, LightComponent specular) : mPos(pos), ambientComponent(ambient), diffuseComponent(diffuse), specularComponent(specular) {}
Light::Light(bool ambient) : ambientComponent(ambient){}
Light::Light(const Light& in) : mPos(in.mPos), ambientComponent(in.ambientComponent), diffuseComponent(in.diffuseComponent), specularComponent(in.specularComponent) {}

Light::~Light(){}

LightComponent Light::getLight(const Vec3D& pos, const Vec3D& cameraPos, const Vec3D& normal) const
{
    Vec3D norm = normal.unitVector();
    Vec3D AB = (mPos-pos).unitVector();
    float deltaA = AB.dotProduct(norm);
    if (deltaA < 0) deltaA = 0;
    Vec3D ref = norm * (2*(norm.dotProduct(AB))) - AB;
    float deltaB = ref.dotProduct((cameraPos-pos).unitVector());
    if (deltaB < 0) deltaB = 0;
    deltaB = powf(deltaB,16);
    return ambientComponent + diffuseComponent*deltaA + specularComponent*deltaB;
}

void Light::setLight(Vec3D pos, LightComponent ambient, LightComponent diffuse, LightComponent specular)
{
    mPos = pos;
    this->ambientComponent = ambient;
    this->diffuseComponent = diffuse;
    this->specularComponent = specular;
}