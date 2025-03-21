#pragma once

#include "tve_window.hpp"

namespace tve
{
class App
{
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

  private:
    TveWindow tveWindow{WIDTH, HEIGHT, "Vulkan"};
};
} // namespace tve
