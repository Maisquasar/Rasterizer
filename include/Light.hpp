#pragma once

#include <maths.hpp>

#define LightComponent Vec3D

class Light
{
public:
    Vec3D mPos;
    LightComponent ambientComponent;
    LightComponent diffuseComponent;
    LightComponent specularComponent;

    //Constructors for light
    Light(Vec3D pos, LightComponent ambient, LightComponent diffuse, LightComponent specular);
    Light(bool ambient = true);
    Light(const Light& in);

    ~Light();

    // Returns the light component of light
    LightComponent getLight(const Vec3D& pos, const Vec3D& cameraPos, const Vec3D& normal) const;
    // Set the light component of light
    void setLight(Vec3D pos, LightComponent ambient, LightComponent diffuse, LightComponent specular);
};
