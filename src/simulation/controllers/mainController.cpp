#include <simulation/controllers/mainController.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/gtc/matrix_transform.hpp>


MainController::MainController(const int width, const int height, GLFWwindow *window):
    camera({
        .target = glm::vec3(0.f),
        .viewportWidth = width,
        .viewportHeight = height,
        .fov = std::numbers::pi_v<float> / 4.f,
        .nearPlane = 0.1f,
        .farPlane = 100.0f,
    }),
    cubeMap(
        "../../resources/textures/MountainPath/posx.jpg",
        "../../resources/textures/MountainPath/negx.jpg",

        "../../resources/textures/MountainPath/posy.jpg",
        "../../resources/textures/MountainPath/negy.jpg",

        "../../resources/textures/MountainPath/posz.jpg",
        "../../resources/textures/MountainPath/negz.jpg"
    )
{
    const auto glsl_version = "#version 410";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    UpdateCubeMesh();
}


MainController::~MainController()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void MainController::Render()
{
    // Creating new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    skyboxShader.Use();
    skyboxShader.SetProjectionMatrix(camera.ProjectionMatrix());
    skyboxShader.SetViewMatrix(camera.ViewMatrix());

    cubeMesh.Use();
    glDrawElements(GL_TRIANGLES, cubeMesh.GetElementsCnt(), GL_UNSIGNED_INT, nullptr);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void MainController::SizeChanged(const int width, const int height)
{
    glViewport(0, 0, width, height);
    camera.SetViewportSize(width, height);
}


void MainController::MouseClicked(const MouseButton button)
{
    mouseState.ButtonClicked(button);
}


void MainController::MouseReleased(const MouseButton button)
{
    mouseState.ButtonReleased(button);
}


void MainController::MouseMoved(const float x, const float y)
{
    mouseState.Moved(x, y);

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (mouseState.IsButtonClicked(Middle)) {
        const auto offset = mouseState.TranslationGet();
        RotateCamera(offset.x * 0.02f, offset.y * 0.02f);
    }
}


void MainController::ScrollMoved(const int offset)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    float val = offset * 0.7f;

    if (val < 0.0f) {
        val = -1.0f / val;
    }
}


void MainController::UpdateCubeMesh()
{
    const std::vector vertices {
        // Vertex 1
        1.0f,
        1.0f,
        1.0f,

        // Vertex 2
        1.0f,
        -1.0f,
        1.0f,

        // Vertex 3
        1.0f,
        -1.0f,
        -1.0f,

        // Vertex 4
        1.0f,
        1.0f,
        -1.0f,

        // Vertex 5
        -1.0f,
        1.0f,
        1.0f,

        // Vertex 6
        -1.0f,
        -1.0f,
        1.0f,

        // Vertex 7
        -1.0f,
        -1.0f,
        -1.0f,

        // Vertex 8
        -1.0f,
        1.0f,
        -1.0f
    };

    const std::vector<uint32_t> indices {
        0, 2, 1,
        0, 3, 2,
        4, 5, 6,
        4, 6, 7,
        4, 1, 5,
        4, 0, 1,
        7, 6, 2,
        7, 2, 3,
        7, 0, 4,
        7, 3, 0,
        5, 1, 6,
        6, 1, 2
    };

    cubeMesh.Update(vertices, indices);
}


void MainController::RotateCamera(float x, float y)
{
    const auto oldPos = camera.GetPosition();

    auto rotation = glm::rotate(glm::mat4(1.f), x, glm::vec3(0.f, 1.f, 0.f));

    const auto axis = glm::cross(oldPos, glm::vec3(0.f, 1.f, 0.f));
    rotation = glm::rotate(rotation, y, axis);

    const auto newPos = rotation * glm::vec4(oldPos, 1.f);
    camera.SetPosition(glm::vec3(newPos.x, newPos.y, newPos.z));
}
