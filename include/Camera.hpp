#pragma once

struct CameraInputs
{
    float deltaX;
    float deltaY;
    bool forward;
    bool backward;
    bool up;
    bool down;
    bool right;
    bool left;
    int scaleMode;
    float scroll;
    bool ortho;
    bool shift;
};

class Mat4D;

class Camera
{
public:
    Vec3D position;
    Vec3D focus;
    Vec3D up = Vec3D(0,1,0);
    Vec2D rotation;
    float distance = 5;
    float fov = 70;
    float near = 0.1f;
    float far = 100.0f;
    float aspect_ratio = 4.0f/3.0f;

    Camera();
    ~Camera();

    void Update(const float p_deltaTime, const CameraInputs& p_inputs);
    // Return the Mat4D view matrix from the camera.
    Mat4D GetViewMatrix();
    // Return the Mat4D projection matrix from the camera.
    Mat4D GetProjection(const int& mode);
    // Return the Mat4D orthographic matrix from the camera.
    Mat4D GetOrtho(const int& mode);

    // Show the list of setting for lock camera.
    void ShowImGuiControls(CameraInputs& p_inputs);
};