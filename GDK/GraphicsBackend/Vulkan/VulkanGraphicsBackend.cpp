//=============================================================================
// VulkanGraphicsBackend
// Reza Nourai, 2016
//=============================================================================
#pragma once

#include "Precomp.h"
#include "VulkanGraphicsBackend.h"

VulkanGraphicsBackend::~VulkanGraphicsBackend()
{
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

bool VulkanGraphicsBackend::InitializeSystem()
{
    VkInstance instance = nullptr;
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS)
    {
        assert(false);
        return false;
    }

    VkPhysicalDevice devices[16]{};
    uint32_t deviceCount = _countof(devices);

    result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
    if (result != VK_SUCCESS)
    {
        assert(false);
        return false;
    }

    physical_device_ = devices[0];

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    result = vkCreateDevice(physical_device_, &deviceCreateInfo, nullptr, &device_);
    if (result != VK_SUCCESS)
    {
        assert(false);
        return false;
    }

    return true;
}

