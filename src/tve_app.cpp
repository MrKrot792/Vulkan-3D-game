#include "tve_app.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace tve
{

App::App()
{
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

App::~App()
{
    vkDestroyPipelineLayout(tveDevice.device(), pipelineLayout, nullptr);
}

void App::run()
{
    while (!tveWindow.shouldClose())
    {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(tveDevice.device());
}

void App::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;

    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(tveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void App::createPipeline()
{
    auto pipelineConfig = TvePipeline::defaultPipelineConfigInfo(tveSwapChain.width(), tveSwapChain.height());
    pipelineConfig.renderPass = tveSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    tvePipeline = std::make_unique<TvePipeline>(tveDevice, "shaders/simple_shader.vert.spv",
                                                "shaders/simple_shader.frag.spv", pipelineConfig);
}

void App::createCommandBuffers()
{
    commandBuffers.resize(tveSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    allocInfo.commandPool = tveDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(tveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers");
    }

    for (int i = 0; i < commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = tveSwapChain.getRenderPass();
        renderPassInfo.framebuffer = tveSwapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = tveSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0] = {0.1f, 0.1f, 0.1f, 1.0f}; // НА ЗАМЕТКУ!! {0.408f, 0.471f, 0.427f, 1.0f};
        clearValues[1] = {1.0f, 0.0f};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        tvePipeline->bind(commandBuffers[i]);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record a command buffer");
        }
    }
}
void App::drawFrame()
{
    uint32_t imageIndex;
    auto result = tveSwapChain.acquireNextImage(&imageIndex);

    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    result = tveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if(result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swap chain image");
    }
}

} // namespace tve
