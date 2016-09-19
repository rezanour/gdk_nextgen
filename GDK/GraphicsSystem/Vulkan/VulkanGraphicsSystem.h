//==============================================================================
// VulkanGraphicsSystem.h - Vulkan implementation of GraphicsSystem interface
// Reza Nourai, 2016
//==============================================================================
#pragma once

#include "GraphicsSystem/GraphicsSystem.h"
#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

class VulkanGraphicsSystem : public GraphicsSystem
{
public:
    VulkanGraphicsSystem(const GraphicsSystemCreateParams& params);
    virtual ~VulkanGraphicsSystem();

    virtual bool Initialize() override;

    // Resource management
    virtual std::shared_ptr<GraphicsResource> CreateTexture2D(uint32_t width, uint32_t height) override;

    // Swapchain
    virtual bool Present(bool vsync) override;

private:
    static bool CheckExtensionsInList(const std::vector<const char*>& required_extensions, const std::vector<VkExtensionProperties>& available_extensions);
    static bool CheckInstanceExtensions(const std::vector<const char*>& required_extensions);
    static bool CheckDeviceExtensions(VkPhysicalDevice device, const std::vector<const char*>& required_extensions);

private:
    VkInstance instance_ = nullptr;

    VkPhysicalDevice physical_device_ = nullptr;
    VkDevice device_ = nullptr;
    VkQueue queue_ = nullptr;
    VkCommandPool command_pool_ = nullptr;
    VkCommandBuffer command_buffer_ = nullptr;

    VkSurfaceKHR surface_ = nullptr;
    VkSwapchainKHR swapchain_ = nullptr;
    VkImage swapchain_images_[3]{};
};
