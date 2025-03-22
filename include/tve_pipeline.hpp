#pragma once

#include "tve_engine_device.hpp"

#include <cstdint>
#include <string>
#include <vector>
namespace tve
{

struct PipelineConfigInfo
{
};

class TvePipeline
{
  public:
    TvePipeline(MyEngineDevice &device, const std::string &vertFilepath, const std::string &fragFilepath,
                const PipelineConfigInfo &configInfo);

    ~TvePipeline()
    {
    }

    TvePipeline(const TvePipeline &) = delete;
    void operator=(const TvePipeline &) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

  private:
    static std::vector<char> readFile(const std::string &filepath);

    void createGraphicsPipeline(const MyEngineDevice &device, const std::string &vertFilepath,
                                const std::string &fragFilepath, const PipelineConfigInfo &configInfo);

    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    MyEngineDevice &tveDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
} // namespace tve
