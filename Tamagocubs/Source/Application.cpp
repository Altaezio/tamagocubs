#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Window.h"

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    Window* myWindow = Window::GetInstance();
    GLFWwindow* window = myWindow->window;

    /* Create a windowed mode window and its OpenGL context */
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    myWindow->InitialiseVerticies();
    myWindow->SetNewMiddleTexture("Tardis.jpg");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        myWindow->Draw();
    }

    glfwTerminate();
    return 0;
}
