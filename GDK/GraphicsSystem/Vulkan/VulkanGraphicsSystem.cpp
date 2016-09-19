//==============================================================================
// VulkanGraphicsSystem
// Reza Nourai, 2016
//==============================================================================
#include "Precomp.h"
#include "Util/Log.h"
#include "VulkanGraphicsSystem.h"
#include "VulkanGraphicsResource.h"

VulkanGraphicsSystem::VulkanGraphicsSystem(const GraphicsSystemCreateParams& params)
    : GraphicsSystem(params)
{
}

VulkanGraphicsSystem::~VulkanGraphicsSystem()
{
    if (swapchain_)
    {
        vkDestroySwapchainKHR(device_, swapchain_, nullptr);
        swapchain_ = nullptr;
    }

    if (surface_)
    {
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        surface_ = nullptr;
    }

    if (command_buffer_)
    {
        vkFreeCommandBuffers(device_, command_pool_, 1, &command_buffer_);
        command_buffer_ = nullptr;
    }

    queue_ = nullptr;

    if (command_pool_)
    {
        vkDestroyCommandPool(device_, command_pool_, nullptr);
        command_pool_ = nullptr;
    }

    if (device_)
    {
        vkDestroyDevice(device_, nullptr);
        device_ = nullptr;
    }

    if (instance_)
    {
        vkDestroyInstance(instance_, nullptr);
        instance_ = nullptr;
    }
}

bool VulkanGraphicsSystem::Initialize()
{
    const std::vector<const char*> required_instance_extensions
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };

    const std::vector<const char*> required_device_extensions
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    if (!CheckInstanceExtensions(required_instance_extensions))
    {
        return false;
    }

    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.enabledExtensionCount = (uint32_t)required_instance_extensions.size();
    instance_create_info.ppEnabledExtensionNames = required_instance_extensions.data();
    VkResult result = vkCreateInstance(&instance_create_info, nullptr, &instance_);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to create instance (%d).\n", result);
        return false;
    }

    VkPhysicalDevice devices[16]{};
    uint32_t device_count = _countof(devices);
    result = vkEnumeratePhysicalDevices(instance_, &device_count, devices);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to enumerate physical devices (%d).\n", result);
        return false;
    }

    // TODO: pick the proper device instead of always assuming 0

    physical_device_ = devices[0];

    if (!CheckDeviceExtensions(physical_device_, required_device_extensions))
    {
        return false;
    }

    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueCount = 1;

    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.enabledExtensionCount = (uint32_t)required_device_extensions.size();
    device_create_info.ppEnabledExtensionNames = required_device_extensions.data();
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    result = vkCreateDevice(physical_device_, &device_create_info, nullptr, &device_);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to create device (%d).\n", result);
        return false;
    }

    vkGetDeviceQueue(device_, 0, 0, &queue_);

    VkCommandPoolCreateInfo command_pool_create_info{};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    result = vkCreateCommandPool(device_, &command_pool_create_info, nullptr, &command_pool_);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to create command pool (%d).\n", result);
        return false;
    }

    VkCommandBufferAllocateInfo command_buffer_allocate_info{};
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool = command_pool_;
    command_buffer_allocate_info.commandBufferCount = 1;
    result = vkAllocateCommandBuffers(device_, &command_buffer_allocate_info, &command_buffer_);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to allocate command buffer (%d).\n", result);
        return false;
    }

    VkWin32SurfaceCreateInfoKHR surface_create_info{};
    surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surface_create_info.hinstance = static_cast<HINSTANCE>(params_.Instance);
    surface_create_info.hwnd = static_cast<HWND>(params_.Window);
    result = vkCreateWin32SurfaceKHR(instance_, &surface_create_info, nullptr, &surface_);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to create surface (%d).\n", result);
        return false;
    }

    VkSwapchainCreateInfoKHR swapchain_create_info{};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface = surface_;
    swapchain_create_info.minImageCount = 2;
    swapchain_create_info.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
    swapchain_create_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapchain_create_info.imageExtent.width = params_.BackBufferWidth;
    swapchain_create_info.imageExtent.height = params_.BackBufferHeight;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    result = vkCreateSwapchainKHR(device_, &swapchain_create_info, nullptr, &swapchain_);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to create swapchain (%d).\n", result);
        return false;
    }

    uint32_t swapchain_image_count = _countof(swapchain_images_);
    result = vkGetSwapchainImagesKHR(device_, swapchain_, &swapchain_image_count, swapchain_images_);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to get swapchain images (%d).\n", result);
        return false;
    }

    return true;
}

std::shared_ptr<GraphicsResource> VulkanGraphicsSystem::CreateTexture2D(uint32_t width, uint32_t height)
{
    GDK_UNUSED(width);
    GDK_UNUSED(height);

    return std::make_shared<VulkanGraphicsResource>(shared_from_this());
}

bool VulkanGraphicsSystem::Present(bool vsync)
{
    GDK_UNUSED(vsync);

    uint32_t index = 0;
    VkResult result = vkAcquireNextImageKHR(device_, swapchain_, 500000, VK_NULL_HANDLE, VK_NULL_HANDLE, &index);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to acquire next image (%d).\n", result);
        return false;
    }

    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    result = vkBeginCommandBuffer(command_buffer_, &begin_info);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to begin command buffer (%d).\n", result);
        return false;
    }

    VkClearColorValue clearValue{};
    clearValue.float32[0] = 0.f;
    clearValue.float32[1] = 0.f;
    clearValue.float32[2] = 1.f;
    clearValue.float32[3] = 1.f;

    VkImageSubresourceRange range{};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.layerCount = 1;
    range.levelCount = 1;
    vkCmdClearColorImage(command_buffer_, swapchain_images_[index], VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &range);

    result = vkEndCommandBuffer(command_buffer_);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to end command buffer (%d).\n", result);
        return false;
    }

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer_;
    result = vkQueueSubmit(queue_, 1, &submit_info, VK_NULL_HANDLE);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to submit command buffer (%d).\n", result);
        return false;
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain_;
    present_info.pImageIndices = &index;
    result = vkQueuePresentKHR(queue_, &present_info);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to present (%d).\n", result);
        return false;
    }

    return true;
}

bool VulkanGraphicsSystem::CheckExtensionsInList(const std::vector<const char*>& required_extensions, const std::vector<VkExtensionProperties>& available_extensions)
{
    size_t extensions_found = 0;
    for (auto& required_extension : required_extensions)
    {
        for (auto& available_extension : available_extensions)
        {
            if (strcmp(required_extension, available_extension.extensionName) == 0)
            {
                ++extensions_found;
                break;
            }
        }
    }
    return extensions_found == required_extensions.size();
}

bool VulkanGraphicsSystem::CheckInstanceExtensions(const std::vector<const char*>& required_extensions)
{
    uint32_t extension_count = 0;
    VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to get extension count (%d).\n", result);
        return false;
    }

    std::vector<VkExtensionProperties> extensions(extension_count);
    result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to enumerate extensions (%d).\n", result);
        return false;
    }

    return CheckExtensionsInList(required_extensions, extensions);
}

bool VulkanGraphicsSystem::CheckDeviceExtensions(VkPhysicalDevice device, const std::vector<const char*>& required_extensions)
{
    uint32_t extension_count = 0;
    VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to get extension count (%d).\n", result);
        return false;
    }

    std::vector<VkExtensionProperties> extensions(extension_count);
    result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extensions.data());
    if (result != VK_SUCCESS)
    {
        LOG_ERROR(L"Failed to enumerate extensions (%d).\n", result);
        return false;
    }

    return CheckExtensionsInList(required_extensions, extensions);
}
