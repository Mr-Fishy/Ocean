#pragma once

#include "Ocean/Core/Types/Integers.hpp"

// libs
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace Ocean {

    namespace Vulkan {

        // Structure Types

        // Memory

        struct MemoryAllocation : public VkMemoryAllocateInfo {
            MemoryAllocation() = default;
            MemoryAllocation(u64 size, u32 memoryType, void* next = nullptr) {
                sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                pNext = next;
                allocationSize = size;
                memoryTypeIndex = memoryType;
            }

        };  // MemoryAllocation

        // Buffers

        struct ExclusiveBufferCreation : public VkBufferCreateInfo {
            ExclusiveBufferCreation() {
                sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                
            }
            /**
             * @brief Construct a new Exclusive Buffer Creation object.
             * 
             * @param bufferSize Specifies the size of the buffer in bytes.
             * @param bufferUsage Specifies the usage of the buffer.
             * @param createFlags TODO
             * @param familyIndexCount TODO
             * @param familyIndices TODO
             * @param next TODO
             */
            ExclusiveBufferCreation(u64 bufferSize, u32 bufferUsage, u32 createFlags = 0, u32 familyIndexCount = 0, u32* familyIndices = nullptr, void* next = nullptr) {
                sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                pNext = next;
                flags = createFlags;
                size = bufferSize;
                usage = bufferUsage;
                sharingMode = VK_SHARING_MODE_EXCLUSIVE;
                queueFamilyIndexCount = familyIndexCount;
                pQueueFamilyIndices = familyIndices;
            }

        };  // BufferCreation

    }   // Vulkan

}   // Ocean