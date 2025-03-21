#include "tve_app.hpp"
#include <GLFW/glfw3.h>

namespace tve
{
void App::run()
{
    while (!tveWindow.shouldClose())
    {
        glfwPollEvents();
    }
}
} // namespace tve
