#pragma once

#include "tve_engine_device.hpp"

#include <cstdint>
#include <string>
#include <vector>
namespace tve
{

struct PipelineConfigInfo
{
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class TvePipeline
{
  public:
    TvePipeline(TveDevice &device, const std::string &vertFilepath, const std::string &fragFilepath,
                const PipelineConfigInfo &configInfo);

    ~TvePipeline();

    TvePipeline(const TvePipeline &) = delete;
    void operator=(const TvePipeline &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

  private:
    static std::vector<char> readFile(const std::string &filepath);

    void createGraphicsPipeline(const TveDevice &device, const std::string &vertFilepath,
                                const std::string &fragFilepath, const PipelineConfigInfo &configInfo);

    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    TveDevice &tveDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
} // namespace tve
