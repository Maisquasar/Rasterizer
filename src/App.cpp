#include <chrono>
#include <App.hpp>
#include <Renderer.hpp>
#include <Framebuffer.hpp>
#include <maths.hpp>
#include <Camera.hpp>
#include <Scene.hpp>
#include <Shader.hpp>

#define MAX_GSHADER 14
#define MAX_FSHADER 4

#ifdef WINDOWS
int strcasecmp(const char* a, const char* b) {
    int ca, cb;
    do {
        ca = *(unsigned char*)a;
        cb = *(unsigned char*)b;
        ca = tolower(toupper(ca));
        cb = tolower(toupper(cb));
        a++;
        b++;
    } while (ca == cb && ca != '\0');
    return ca - cb;
}
#endif

//The text for msaa
const char *const msaaDesc[4] =
    {
        "MSAA : Off",
        "MSAA : 4x",
        "MSAA : 8x",
        "MSAA : 16x"};

//The text for shaders
const char *const shaderList[MAX_FSHADER] =
    {
        "Default Shader",
        "Waves Shader",
        "Cel Shader",
        "Glitch Shader",
        };

const char *const FilterDesc[MAX_GSHADER] =
    {
        "No Filter",
        "Inverted",
        "Red Only",
        "Green Only",
        "Blue Only",
        "Pixelizer",
        "Creeper view",
        "Posterize",
        "GBC",
        "UpsideDown",
        "GrayScale",
        "Hash",
        "VHS",
        "Pumpkin",
        };

static float mouseScroll = 0;

//Get back Events and setup ImGUI frame
void App::NewFrame(bool mouseCaptured)
{
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    if (mouseCaptured)
        ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    ImGui::NewFrame();
}

//Clear buffer et render ImGUI
void App::EndFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void App::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    mouseScroll = yoffset;
}

//Constructor with Init struct
App::App(const AppInit &p_init)
{
    //Clear search variable
    search[0] = '\0';
    appRng = PRNG();

    // Setup glfw
    glfwSetErrorCallback(p_init.ErrorCallback);
    if (!glfwInit())
    {
        printf("glfwInit failed\n");
        state = GLFWFAILED;
        return;
    }

    //Create Window
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    window = glfwCreateWindow(p_init.width, p_init.height, p_init.title, nullptr, nullptr);
    if (window == nullptr)
    {
        printf("glfwCreateWindow failed\n");
        state = WINDOWFAILED;
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync

    // Setup glad
    if (gladLoadGL((GLADloadfunc)glfwGetProcAddress) == 0)
    {
        printf("gladLoaderLoadGL failed\n");
        state = GLADFAILED;
        return;
    }

    glfwSetScrollCallback(window, scroll_callback);

    // Setup KHR debug
    if (GLAD_GL_KHR_debug)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(p_init.DebugMessageCallback, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
    }

    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    state = SUCCESS;
}

//Update device and call the renderer
void App::Update()
{
    // Create renderer framebuffer (color+depth+opengl texture)
    // We need an OpenGL texture to display the result of the renderer to the screen
    Framebuffer framebuffer(800, 600);
    // Init renderer
    Renderer renderer(
        framebuffer.GetColorBuffer(),
        framebuffer.GetDepthBuffer(),
        framebuffer.GetWidth(), framebuffer.GetHeight());

    Scene scene(renderer);
    int texId = scene.getTextureIdByName("crate");
    if (texId != -1)
    {
        Texture* tex = &scene.mTexture[texId];
        GLFWimage img = {tex->texturesize.x,tex->texturesize.y, reinterpret_cast<unsigned char*>(tex->textureBuffer)};
        glfwSetWindowIcon(window,1,&img);
    }
    texId = scene.getTextureIdByName("pumpkinblur");
    if (texId != -1)
    {
        shader::setPumpkinTexture(&scene.mTexture[texId]);
    }

    CameraInputs inputs = {0};
    Camera camera;
    camera.position = Vec3D(0, 0, 5);

    //reset all value
    bool mouseCaptured = false;
    double mouseX = 0.0;
    double mouseY = 0.0;
    float mouseDeltaX = 0.0;
    float mouseDeltaY = 0.0;
    int tmpMsaa = 1;
    float fps = 0;
    float minFps = 0;
    float maxFps = 0;
    scene.UpdateVersion(1, renderer, inputs, tmpMsaa);

    //App loop
    while (glfwWindowShouldClose(window) == false)
    {
        NewFrame(mouseCaptured); //ImGui

        {
            double newMouseX, newMouseY;
            glfwGetCursorPos(window, &newMouseX, &newMouseY);
            mouseDeltaX = (float)(newMouseX - mouseX);
            mouseDeltaY = (float)(newMouseY - mouseY);
            mouseX = newMouseX;
            mouseY = newMouseY;
        }

        // Update camera
        if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE))
        {
            mouseCaptured = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        // F1 for Wireframe
        if (ImGui::IsKeyPressed(GLFW_KEY_F1, false))
            renderer.WireFrame = !renderer.WireFrame;
        // F2 for msaa
        if (ImGui::IsKeyPressed(GLFW_KEY_F2, false))
            tmpMsaa = (renderer.msaa == 1) ? 2 : 1;

        if (mouseCaptured)
        {
            inputs.deltaX = mouseDeltaX;
            inputs.deltaY = mouseDeltaY;
            inputs.forward = ImGui::IsKeyDown(GLFW_KEY_W);
            inputs.backward = ImGui::IsKeyDown(GLFW_KEY_S);
            inputs.right = ImGui::IsKeyDown(GLFW_KEY_D);
            inputs.left = ImGui::IsKeyDown(GLFW_KEY_A);
            inputs.up = ImGui::IsKeyDown(GLFW_KEY_E);
            inputs.down = ImGui::IsKeyDown(GLFW_KEY_Q);
            inputs.shift = ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT);
            inputs.scroll = mouseScroll;
            camera.Update(ImGui::GetIO().DeltaTime, inputs);
            mouseScroll = 0;
        }

        // Clear buffers
        framebuffer.Clear();

        // Setup matrices
        Mat4D projection = inputs.ortho ? camera.GetOrtho(inputs.scaleMode) : camera.GetProjection(inputs.scaleMode);
        Mat4D view = camera.GetViewMatrix();
        renderer.SetProjection(&projection);
        renderer.SetView(&view);
        renderer.cameraPos = camera.position;
        using namespace std::chrono;
        renderer.deltaTime = scene.time;

        // Render scene
        scene.Update(ImGui::GetIO().DeltaTime, renderer, camera.focus, inputs);

        // Upload texture
        framebuffer.UpdateTexture(renderer.msaa);
        framebuffer.time = scene.time;
        shader::incrementDec(scene.time);
        // Fps updater
        fps = 1 / ImGui::GetIO().DeltaTime;
        if ((int)(scene.time * 20) != (int)((scene.time + ImGui::GetIO().DeltaTime) * 20))
        {
            framebuffer.rngState = appRng.getRandomNumber();
        }
        if ((int)(scene.time / 5) != (int)((scene.time + ImGui::GetIO().DeltaTime) / 5))
        {
            minFps = fps;
            maxFps = fps;
        }
        else
        {
            if (minFps > fps)
                minFps = fps;
            if (maxFps < fps)
                maxFps = fps;
        }

        // Display debug controls
        if (ImGui::Begin("Config"))
        {
            //FrameBuffer Overlay
            if (ImGui::CollapsingHeader("Framebuffer", ImGuiTreeNodeFlags_DefaultOpen))
            {
                Vec4D color = Vec4D(framebuffer.clearColor);
                ImGui::ColorEdit4("clearColor", &color.x);
                framebuffer.clearColor = UChar4D(color);
            }
            //Camera Overlay
            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
            {
                camera.ShowImGuiControls(inputs);
                ImGui::Checkbox("Wireframe render", &renderer.WireFrame);
                ImGui::Checkbox("Orthographic Projection", &inputs.ortho);
                ImGui::Checkbox("Corrected Perspective", &renderer.correctPerspective);
                ImGui::Checkbox("Per pixel lightning", &renderer.perPixelLight);
                ImGui::SliderInt(msaaDesc[tmpMsaa - 1], &tmpMsaa, 1, 4);
                ImGui::SliderInt(FilterDesc[framebuffer.shader], &framebuffer.shader, 0, MAX_GSHADER - 1);
            }
            //Draw FPS
            ImGui::Text("FPS: %.1f  Min: %.1f  Max: %.1f", fps, minFps, maxFps);
        }
        ImGui::End();
        //Triangles Overlay
        if (ImGui::Begin("Triangles"))
        {
            ImGui::Text("Total number of Triangles: %d", scene.getNumberOfTriangles());
            ImGui::Text("Number of Triangles: %d", scene.getNumberOfTriangles(true));
            ImGui::Text("Drawn Triangles: %d", renderer.WireFrame ? renderer.TriangleDraw / 2 : renderer.TriangleDraw);
            char c[64];
            bool popup = false;

            if (ImGui::CollapsingHeader("Triangles by Models", ImGuiTreeNodeFlags_Selected))
            {
                //Models loop
                for (size_t i = 0; i < scene.models.size(); i++)
                {
                    //Button to Open Overlay for settings models
                    char c[64];
                    ImGui::PushID(i);
                    sprintf(c, "%s", scene.models[i].getModelName().c_str());
                    if (ImGui::Button(c))
                    {
                        index = i;
                        popup = true;
                    }
                    snprintf(c, 64, ": %s (%d tris)", scene.models[i].meshs[0]->getName().c_str(), scene.models[i].getNumberOfTriangles());
                    ImGui::SameLine();
                    ImGui::Text(c);
                    ImGui::PopID();
                }
                // If Button Press, open Popup
                if (popup)
                {
                    ImGui::OpenPopup("Settings");
                }
                if (ImGui::BeginPopupModal("Settings"))
                {
                    // List of Models
                    if (ImGui::BeginCombo("Models", scene.models[index].meshs[0]->getName().c_str()))
                    {
                        for (size_t i = 0; i < scene.models.size(); i++)
                        {
                            if (ImGui::Selectable(scene.models[i].meshs[0]->getName().c_str()))
                            {
                                index = i;
                                scene.models[index].meshs[0]->getName().c_str();
                            }
                        }
                        ImGui::EndCombo();
                    }
                    static bool lockScale = false;
                    //Parameters
                    ImGui::DragFloat3("Position:", &scene.models[index].pos.x, 0.1);
                    ImGui::DragFloat3("Rotation:", &scene.models[index].rot.x, 0.1);
                    if (!lockScale){
                        ImGui::DragFloat3("Scale:", &scene.models[index].scale.x, 0.1);
                    }
                    else{
                        ImGui::DragFloat("Scale:", &scene.models[index].scale.x, 0.05);
                        scene.models[index].scale = {scene.models[index].scale.x};
                    }
                    ImGui::SameLine();
                    ImGui::Checkbox("Lock", &lockScale);
                    Vec4D color = Vec4D(scene.models[index].color);
                    ImGui::ColorEdit4("Color:", &color.x);
                    scene.models[index].color = UChar4D(color);

                    //Meshs loop
                    for (size_t j = 0; j < scene.models[index].meshs.size(); j++)
                    {
                        // Show all Meshs in popup
                        snprintf(c, 64, "%s: %d tris", scene.models[index].meshs[j]->getName().c_str(), scene.models[index].meshs[j]->getNumberOfTriangles());
                        // Check box to draw or not meshs
                        ImGui::Checkbox(c, &scene.models[index].meshs[j]->selected);
                        ImGui::SameLine();
                        // Texture Button
                        if (ImGui::Button("Set Texture"))
                        {
                            ImGui::OpenPopup("Textures");
                            ImGui::SetNextWindowSize(ImVec2(700, 400));
                        }
                        // Popup for Textures
                        if (ImGui::BeginPopupModal("Textures"))
                        {
                            bool close = false; 
                            // Search Bar
                            ImGui::InputText("Search", search, IM_ARRAYSIZE(search));
                            ImGui::SameLine();
                            if (ImGui::Button("Remove Texture"))
                                scene.models[index].meshs[j]->SetTexture(-1);
                            // Variable for count number of texture in line
                            int x = -1;
                            // Show all Textures
                            ImGui::SameLine();
                            // Button for close popup
                            if (ImGui::Button("Close"))
                                close = true;
                            for (size_t k = 0; k < scene.mTexture.size(); k++)
                            {
                                if (close || ImGui::IsKeyPressed(ImGuiKey_Escape))
                                    ImGui::CloseCurrentPopup();
                                std::string tmp;
                                for (int l = 0; l < 32; l++)
                                {
                                    if (search[l] == '\0')
                                        break;
                                    tmp.push_back(scene.mTexture[k].getName()[l]);
                                }
                                if (!strcasecmp(tmp.c_str(), search))
                                {
                                    x++;
                                    // Button texture, when click set current models to texture.
                                    if (ImGui::ImageButton((ImTextureID)(size_t)scene.mTexture[k].getTextureID(), ImVec2{32, 32}))
                                    {
                                        ImGui::CloseCurrentPopup();
                                        scene.models[index].meshs[j]->SetTexture(scene.mTexture[k].getTextureID() - 2);
                                    }
                                    
                                    //Display textures
                                    if (x % 14 == 13)
                                        ImGui::NewLine();
                                    ImGui::SameLine();
                                }
                            }
                            ImGui::EndPopup();
                        }
                    }
                    ImGui::ListBox("Model Shader", &scene.models[index].modelShader, shaderList, MAX_FSHADER);
                    // Press "Close" button or Escape key to close Popup
                    ImGui::Checkbox("Bilinear filtering", &scene.models[index].bilinear);
                    ImGui::SameLine();
                    ImGui::Dummy({100,0});
                    ImGui::SameLine();
                    if (ImGui::Button("Close") || ImGui::IsKeyPressed(ImGuiKey_Escape))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
        }
        ImGui::End();
        // Version overlay
        if (ImGui::Begin("Versions"))
        {
            int tmpVersion = scene.version;
            ImGui::SliderInt("Version", &tmpVersion, 1, Max_version);
            if (tmpVersion != scene.version)
                scene.UpdateVersion(tmpVersion, renderer, inputs, tmpMsaa);
        }
        ImGui::End();
        ImGui::Begin("FrameBuffer");
        ImGui::Text("[%d; %d] (Right click to capture mouse, Esc to un-capture)", framebuffer.GetWidth() / renderer.msaa, framebuffer.GetHeight() / renderer.msaa);
        // Display framebuffer (renderer output)
        ImGui::Image((ImTextureID)(size_t)framebuffer.GetColorTexture(), {(float)framebuffer.GetWidth() / renderer.msaa, (float)framebuffer.GetHeight() / renderer.msaa});
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            mouseCaptured = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        Int2D size = Int2D(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()) - Int2D(16, 52);
        if (size.x < 16)
            size.x = 16;
        if (size.y < 16)
            size.y = 16;

        ImGui::End();
        EndFrame();
        if (!((size * renderer.msaa) == Int2D(framebuffer.GetWidth(), framebuffer.GetHeight())) || renderer.msaa != tmpMsaa)
        {
            renderer.msaa = tmpMsaa;
            framebuffer.setSize(size, renderer.msaa);
            renderer.updateBuffers(framebuffer.GetColorBuffer(), framebuffer.GetDepthBuffer(), size.x, size.y);
            camera.aspect_ratio = size.x * 1.0f / size.y;
        }
        renderer.TriangleDraw = 0;
    }
}

//destructor who kill glfw and clean the window buffer
App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}