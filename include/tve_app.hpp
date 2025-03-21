#pragma once

#include "tve_pipeline.hpp"
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
    TvePipeline tvePipeline{"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
};
} // namespace tve
