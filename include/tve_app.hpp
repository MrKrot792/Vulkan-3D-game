#pragma once

#include "tve_pipeline.hpp"
#include "tve_window.hpp"
#include "tve_engine_device.hpp"
#include "tve_engine_swap_chain.hpp"

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
    TveDevice tveDevice{tveWindow};
    TveSwapChain tveSwapChain{tveDevice, tveWindow.getExtent()};
    TvePipeline tvePipeline{tveDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", TvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};
} // namespace tve
