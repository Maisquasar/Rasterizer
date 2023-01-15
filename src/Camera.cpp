
#include <imgui.h>
#include <maths.hpp>

#include <Camera.hpp>

#define CAM_SPEED 30.0f

const char *const cameraModes[3] =
    {
        "Lock vertical",
        "Lock horizontal",
        "Force 4/3"};

Camera::Camera() {}
Camera::~Camera() {}

void Camera::Update(const float p_deltaTime, const CameraInputs &inputs)
{
    // Update distance.
    distance = math::cut(distance - inputs.scroll / 3, 0.001f, 100.0f);
    // Update rotation.
    rotation = rotation + Vec2D(-inputs.deltaX / 10, inputs.deltaY / 10);
    rotation = Vec2D(math::mod(rotation.x, 360), math::cut(rotation.y, -89.99f, 89.99f));
    // Update focus.
    float dSpeed = p_deltaTime * CAM_SPEED * (inputs.shift ? 5.0f : 1.0f);
    Vec3D delta = Vec3D(dSpeed * inputs.right - dSpeed * inputs.left, dSpeed * inputs.up - dSpeed * inputs.down, dSpeed * inputs.backward - dSpeed * inputs.forward) / 20;
    focus = focus + Vec3D(cosf(math::toRadians(rotation.x)) * delta.x + sinf(math::toRadians(rotation.x)) * delta.z, delta.y, -sinf(math::toRadians(rotation.x)) * delta.x + cosf(math::toRadians(rotation.x)) * delta.z);
    // Update Position.
    position = focus + Vec3D(sinf(math::toRadians(rotation.x)) * cosf(math::toRadians(rotation.y)), sinf(math::toRadians(rotation.y)), cosf(math::toRadians(rotation.x)) * cosf(math::toRadians(rotation.y))) * (distance == 0.0f ? 0.001f : distance);
}

Mat4D Camera::GetViewMatrix()
{
    Mat4D temp;
    Vec3D z = (position - focus).unitVector();
    Vec3D x = up.crossProduct(z).unitVector();
    Vec3D y = z.crossProduct(x);
    Vec3D delta = Vec3D(-x.dotProduct(position), -y.dotProduct(position), -z.dotProduct(position));
    for (int i = 0; i < 3; i++)
    {
        temp.at(i, 0) = x[i];
        temp.at(i, 1) = y[i];
        temp.at(i, 2) = z[i];
        temp.at(3, i) = delta[i];
    }
    temp.at(3, 3) = 1;
    return temp;
}

Mat4D Camera::GetProjection(const int &mode)
{
    float s = 1.0f / atanf(math::toRadians(fov / 2.0f));
    float param1 = -(far + near) / (far - near);
    float param2 = -(2 * near * far) / (far - near);
    Mat4D out;
    out.at(0, 0) = mode == 0 ? s / aspect_ratio : (mode == 1 ? s : s / (4.0f / 3));
    out.at(1, 1) = mode == 1 ? -s * aspect_ratio : -s;
    out.at(2, 2) = param1;
    out.at(2, 3) = -1;
    out.at(3, 2) = param2;
    return out;
}

Mat4D Camera::GetOrtho(const int &mode)
{
    float s = 0.25f / atanf(math::toRadians(fov / 2.0f));
    float param1 = 2 / (far - near);
    float param2 = (far + near) / (far - near);
    Mat4D out;
    out.at(0, 0) = mode == 0 ? s / aspect_ratio : (mode == 1 ? s : s / (4.0f / 3));
    out.at(1, 1) = mode == 1 ? -s * aspect_ratio : -s;
    out.at(2, 2) = param1;
    out.at(3, 3) = 1;
    out.at(3, 2) = param2;
    return out;
}

void Camera::ShowImGuiControls(CameraInputs &inputs)
{
    ImGui::ListBox("Scale modes", &inputs.scaleMode, cameraModes, 3);
}